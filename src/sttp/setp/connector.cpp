//
//  connector.cpp
//  Smobiler
//
//  Created by jinyk on 16/2/29.
//
//

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/randpool.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/asn.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/sha.h>
#include "connector.h"
#include "state.h"

using namespace CryptoPP;

namespace setp
{

    ip::tcp::resolver::iterator ConnectCondition::operator()(const error_code& errorCode,
                                                             const ip::tcp::resolver::iterator& iter)
    {
        //ios下执行resolve转换ipv4为ipv6地址后，port被重置为0。
        //因此需要将转换后的ipv6地址重新做一遍resolve
        if(iter->endpoint().protocol() == ip::tcp::v6())
        {
            struct sockaddr_in6* addr6;
            addr6 = (struct sockaddr_in6*)iter->endpoint().data();
            if(0 == addr6->sin6_port)
            {
                char buf[32];
                const char* pszTemp;
                pszTemp = inet_ntop(AF_INET6, &addr6->sin6_addr, buf, sizeof(buf));
                error_code ec;
                auto itr = ip::tcp::resolver(m_ioService).resolve({pszTemp,m_port}, ec);
                if(!ec) return itr;
            }
        }
        return iter;
    }
    
    SETPConnector::SETPConnector(asio::io_service& ioService)
        : m_ioService(ioService),
          m_stream(ioService),
          m_heartbeatTimer(ioService)
    {
        memset(m_session.getSessionId(), 0, Session::SessionIdLength);
    }
    
    void SETPConnector::close(error_code& error)
    {
        m_stopRead = true;
        m_stream.close(error);
    }
    
    void SETPConnector::_connect(const Handler& handler)
    {
        connect(m_host,m_port,handler);
    }
    
    void SETPConnector::connect(const string & host,
                                std::size_t port,
                                const Handler& handler)
    {
        m_host = host; m_port = port;
        stringstream sstPort;
        sstPort << port;
        error_code ec;
        auto itrEndpoint = ip::tcp::resolver(m_ioService).resolve({host,sstPort.str()}, ec);
        if(ec)
        {
            m_ioService.post([handler,ec](){handler(ec);});
            return;
        }

        asio::async_connect(m_stream,
                            itrEndpoint,
                            ConnectCondition(m_ioService,sstPort.str()),
                            [this,handler](std::error_code error, ip::tcp::resolver::iterator)
                            {
                                if(error)
                                {
                                    handler(error);
                                    return;
                                }
                                HandshakeHello::instance().handshake(*this, handler);
                                m_stopRead = false;
                                _asyncRead();
                            });
    }
    
    void SETPConnector::asyncWrite(const unsigned char* pBuf,
                    const size_t size,
                    const IoHandler&& handler)
    {
        size_t outputSize = AES::BLOCKSIZE * (size/AES::BLOCKSIZE + 1);
        
        if(m_sendBuffer.capacity() < outputSize + Package::head_length)
        {
            m_sendBuffer.reserve( (outputSize + Package::head_length) * 1.1 );
        }
        m_sendBuffer.resize(outputSize + Package::head_length );
        
        if(m_plainSendBuf.capacity() < size)
        {
            m_plainSendBuf.reserve(size);
        }
        m_plainSendBuf.resize(size);
        memcpy(&m_plainSendBuf[0], pBuf, size);
        
        Package pkg(m_sendBuffer);
        pkg.asApplicationData();
        pkg.setLength(outputSize);
        
        try
        {
            //aes加密
            CBC_Mode<AES>::Encryption aes(m_session.getAesKey(),
                                          Session::AesKeyLength,
                                          m_session.getAesIv());
            ArraySource(&m_plainSendBuf[0],
                        size,true,
                        new StreamTransformationFilter(aes,
                                                       new ArraySink(&m_sendBuffer[Package::head_length],
                                                                 outputSize)));
        }
        catch(Exception& ex)
        {
            handler(SetpError::EncryptionError(),0);
            return;
        }
        //发送
        async_write(m_stream,buffer(m_sendBuffer,m_sendBuffer.size()),handler);
    }
    
    void SETPConnector::asyncRead(const IoHandler& handler)
    {
        m_onReceived = handler;
    }
    
    void SETPConnector::_asyncRead()
    {
        if(m_stopRead) return;
        if(m_recvBuffer.size() < Package::head_length) m_recvBuffer.resize(Package::head_length);
        //读取报文
        async_read(m_stream,
                   buffer(&m_recvBuffer[0],Package::head_length),
                   [this](const error_code& error,const size_t bytes_transfered)
                   {
                       if( error || 0 == bytes_transfered )
                       {
                           if(m_onReceived) m_onReceived(error,bytes_transfered);
                           _asyncRead();
                           return;
                       }
                       
                       Package pkg(m_recvBuffer);
                       if(pkg.getLength() > 1024 * 1024 * 1)
                       {
                           //包大于1M报错
                           error_code err = SetpError::UnexpectedMessage();
                           if(m_onReceived) m_onReceived(err,bytes_transfered);
                           return;
                       }
                       if(m_recvBuffer.capacity() < pkg.getLength() + Package::head_length)
                       {
                           m_recvBuffer.reserve( (pkg.getLength()+ Package::head_length) * 1.1 );
                       }
                       m_recvBuffer.resize(pkg.getLength() + Package::head_length);
                       
                       //读取数据部分
                       async_read(m_stream,
                                  buffer(&m_recvBuffer[Package::head_length],pkg.getLength()),
                                  [this](const error_code& error,const size_t bytes_transfered)
                                  {
                                      if( error || 0 == bytes_transfered)
                                      {
                                          if(m_onReceived) m_onReceived(error,bytes_transfered);
                                          _asyncRead();
                                          return;
                                      }
                                      //分发收到的报文至对应处理单元
                                      m_pState->handleReceived(*this, error,
                                                               bytes_transfered);
                                      _asyncRead();
                                  });
                   });
    }
    
    void SETPConnector::changeState(State* pState)
    {
        if(m_pState == pState) return;
        if(m_pState) m_pState->setKeepAlive(*this,false);
        m_pState = pState;
        m_pState->setKeepAlive(*this,m_keepAlive);
    }
    
    void SETPConnector::setKeepAlive(bool keepAlive)
    {
        m_ioService.post([this,keepAlive]()
                         {
                             m_keepAlive = keepAlive;
                             if(m_pState) m_pState->setKeepAlive(*this,keepAlive);
                         });
    }
}//end namespace

