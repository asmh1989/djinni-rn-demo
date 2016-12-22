//
//  state.cpp
//  Smobiler
//
//  Created by jinyk on 16/11/25.
//
//

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

#include "log_impl.hpp"

using namespace smobiler;

namespace setp
{
    static const long long s_heartbeatDuration = 10000; //毫秒
    
    HandshakeHello& HandshakeHello::instance()
    {
        static HandshakeHello instance;
        return instance;
    }
    
    void HandshakeHello::handshake(SETPConnector& connector,
                                   const _Handler& handler)
    {
        m_handler = [handler](const error_code& error,const size_t)
        {
            handler(error);
        };
        
        HandshakeHelloPackage clientHelloPkg(m_sendBuffer);
        clientHelloPkg.asClient();
        
        default_random_engine generator;
        std::uniform_int_distribution<uint32_t> distribution(0, UINT32_MAX );
        uint32_t randomValue = distribution(generator);
        clientHelloPkg.setRandom(randomValue);
        connector.m_session.setRandomKey1(randomValue);
        memcpy(clientHelloPkg.getSessionId(),
               connector.m_session.getSessionId(),
               Session::SessionIdLength);
        
        async_write(connector.m_stream,
                    buffer(&m_sendBuffer[0],m_sendBuffer.size()),
                    [&connector,&handler](const error_code& error,
                                         size_t bytes_transfered)
                    {
                        if(error && handler)
                        {
                            handler(error);
                            return;
                        }
                        connector.changeState(&HandshakeHello::instance());
                    });
    }
    
    bool HandshakeHello::handleReceived(SETPConnector& connector,
                                        const error_code& error,
                                        size_t bytes_transfered)
    {
        if(error && m_handler)
        {
            m_handler(error,0);
            return true;
        }
        
        HandshakeHelloPackage pkg(connector.m_recvBuffer);
        if(pkg.getContentType()!= Package::ContentType::Handshake)
        {
            if(m_handler) m_handler(SetpError::UnexpectedMessage(),0);
            return true;
        }
        if(pkg.getMessageType() == HandshakeHelloPackage::MessageType::ServerConfirm)
        {
            connector.changeState(&HandshakeFinish::instance());
            error_code ec;
            if(m_handler) m_handler(ec,bytes_transfered);
            LOGD(TAG_SOCKET,"Handshaker resume success.");

            return true;
        }
        
        if(pkg.getMessageType() != HandshakeHelloPackage::MessageType::ServerHello)
        {
            if(m_handler) m_handler(SetpError::UnexpectedMessage(),0);
            return true;
        }
        
        connector.m_session.setRandomKey2(pkg.getRandom());
        connector.m_session.setVersion(pkg.getMajorVersion(),pkg.getMinorVersion());
        memcpy(connector.m_session.getSessionId(),
               pkg.getSessionId(),
               Session::SessionIdLength);
        
        ArraySource pubKeyBuf(pkg.getRsaKey(), pkg.getRsaKeyLength(), true);
        try
        {
            connector.m_session.setPublicKey(pubKeyBuf);
        }
        catch (BERDecodeErr& ex)
        {
            if(m_handler) m_handler(SetpError::PubKeyError(),0);
            return true;
        }
        
        HandshakeConfirm::instance().handshake(connector,
                                               [this](const error_code& err)
                                               {
                                                   if(m_handler) m_handler(err,0);
                                               });
        return true;
    }
    
    /////////////////////////////////////////////////
    
    HandshakeConfirm& HandshakeConfirm::instance()
    {
        static HandshakeConfirm instance;
        return instance;
    }
    
    void HandshakeConfirm::handshake(SETPConnector& connector,
                                     const _Handler& handler)
    {
        m_handler = [handler](const error_code& error,const size_t)
        {
            handler(error);
        };
        
        HandshakeConfirmPackage clientConfirmPkg(m_sendBuffer);
        clientConfirmPkg.asClient();
        
        default_random_engine generator;
        std::uniform_int_distribution<uint32_t> distribution(0, UINT32_MAX );
        uint32_t random3 = distribution(generator);
        connector.getSession().setRandomKey3(random3);
        
        //加密random3
        try
        {
            AutoSeededRandomPool randPool;
            ArraySource(connector.getSession().getRandomKey3(),
                        Session::RandomKeyLength,
                        true,
                        new PK_EncryptorFilter(randPool,
                                               connector.m_session.getPublicKey(),
                                               new ArraySink(clientConfirmPkg.getPremasteKey(),
                                                             Session::PublicKeyLength)));
        }
        catch(Exception& ex)
        {
            handler(SetpError::EncryptionError());
            return;
        }
        //发送confirm包
        async_write(connector.m_stream,
                    buffer(&m_sendBuffer[0],m_sendBuffer.size()),
                    [&connector,&handler](const error_code& error,size_t bytes_transfered)
                    {
                        if(error)
                        {
                            handler(error);
                            return;
                        }
                        connector.changeState(&HandshakeConfirm::instance());
                    });
    }
    
    bool HandshakeConfirm::handleReceived(SETPConnector& connector,
                                          const error_code& error,
                                          size_t bytes_transfered)
    {
        if(error && m_handler)
        {
            m_handler(error,0);
            return true;
        }
        
        HandshakeConfirmPackage pkg(connector.m_recvBuffer);
        if(pkg.getContentType()!= Package::ContentType::Handshake)
        {
            if(m_handler) m_handler(SetpError::UnexpectedMessage(),bytes_transfered);
            return true;
        }
        
        if(pkg.getMessageType() != HandshakeConfirmPackage::MessageType::ServerConfirm)
        {
            if(m_handler) m_handler(SetpError::UnexpectedMessage(),0);
            return true;
        }
        
        HandshakeFinish::instance().handshake(connector,
                                              [this](const error_code& err)
                                              {
                                                  if(m_handler) m_handler(err,0);
                                              });
        return true;
    }
    
    /////////////////////////////////////////////////
    
    HandshakeFinish& HandshakeFinish::instance()
    {
        static HandshakeFinish instance;
        return instance;
    }
    
    void HandshakeFinish::handshake(SETPConnector& connector,
                                    const _Handler& handler)
    {
        //生成aes_iv
        Weak1::MD5 md5;
        SecByteBlock digest(md5.DigestSize());
        md5.Update(connector.getSession().getRandomKey(), Session::RandomKeyLength*3);
        md5.Final(digest);
        memcpy(connector.getSession().getAesIv(), digest, md5.DigestSize());
        
        //生成aes_key
        SHA256 sha256;
        SecByteBlock digest256(sha256.DigestSize());
        sha256.Update(connector.getSession().getRandomKey(), Session::RandomKeyLength*3);
        sha256.Final(digest256);
        memcpy(connector.getSession().getAesKey(), digest256, sha256.DigestSize());
        
        //修改状态
        connector.changeState(&HandshakeFinish::instance());
        
        error_code ec;
        if(handler) handler(ec);
        
        LOGD(TAG_SOCKET,"Handshaker success.");
    }
    
    bool HandshakeFinish::handleReceived(SETPConnector& connector,
                                         const error_code& error,
                                         size_t bytes_transfered)
    {
        if(error)
        {
            connector.m_onReceived(error,bytes_transfered);
            return true;
        }
        
        Package pkg(connector.m_recvBuffer);
        if(pkg.getContentType() == Package::ContentType::Heartbeat)
        {
            HeartbeatPackage heartbeatPkg(m_sendBuffer);
            if(heartbeatPkg.getData() == m_heartbeatNum)
            {
                connector.m_heartbeatTimer.cancel();
            }
            return false;
        }
        if(pkg.getContentType()!= Package::ContentType::ApplicationData)
        {
            connector.changeState(&ErrorState::instance());
            ErrorState::instance().handleReceived(connector,
                                                  error,
                                                  bytes_transfered);
            return true;
        }
        
        //aes 解密
        try
        {
            CBC_Mode<AES>::Decryption aes(connector.getSession().getAesKey(),
                                          Session::AesKeyLength,
                                          connector.getSession().getAesIv());
            auto sink = new ArraySink(&connector.m_recvBuffer[Package::head_length],
                                      bytes_transfered);
            ArraySource(&connector.m_recvBuffer[Package::head_length],
                        bytes_transfered,true,
                        new StreamTransformationFilter(aes,sink));
        }
        catch(Exception& ex)
        {
           if(connector.m_onReceived) connector.m_onReceived(SetpError::DecryptionError(),bytes_transfered);
        }
        if(connector.m_onReceived) connector.m_onReceived(error,bytes_transfered);
        return true;
    }
    
    void HandshakeFinish::setKeepAlive(SETPConnector& connector,bool keepAlive)
    {
        using namespace std::chrono;
        m_keepAlive = keepAlive;
        if(m_keepAlive)
        {
            //开始heartbeat
            m_heartbeatEcho = true;
            connector.m_heartbeatTimer.expires_from_now(milliseconds(s_heartbeatDuration));
            startHeartbeat(connector);
        }
    }
    
    void HandshakeFinish::startHeartbeat(SETPConnector& connector)
    {
        using namespace std::chrono;
        if(!m_keepAlive) return;
        connector.m_heartbeatTimer.async_wait([&connector,this](const asio::error_code& error)
        {
            static steady_clock::time_point heartbeatStart;
            
            if(asio::error::operation_aborted == error)
            {
                //收到heartbeat回显
                //CCLog("-----------heartbeat echo %ui",m_heartbeatNum);
                m_heartbeatEcho = true;
                auto echoDuration = duration_cast<milliseconds>(steady_clock::now() -
                                                                heartbeatStart).count();
                connector.m_heartbeatTimer.expires_from_now(milliseconds(s_heartbeatDuration -
                                                                         echoDuration ));
                startHeartbeat(connector);
            }
            else
            {
                if(m_heartbeatEcho)  //发送间隔超时
                {
                    HeartbeatPackage heartbeatPkg(m_sendBuffer);
                    heartbeatPkg.asClient();
                    static default_random_engine generator;
                    static std::uniform_int_distribution<uint32_t> distribution(0, UINT32_MAX );
                    m_heartbeatNum = distribution(generator);
                    heartbeatPkg.setData(m_heartbeatNum);
                    async_write(connector.m_stream,
                                buffer(&m_sendBuffer[0],m_sendBuffer.size()),
                                [&connector,this](const error_code&,size_t)
                                {
                                    //CCLog("------------send heartbeat");
                                    m_heartbeatEcho = false;
                                    heartbeatStart = steady_clock::now();
                                    connector.m_heartbeatTimer.expires_from_now(milliseconds(s_heartbeatDuration));
                                    startHeartbeat(connector);
                                });
                }
                else //回显超时
                {
                    //CCLog("---------------heartbeat timeout");
                    if(m_keepAlive)
                    {
                        connector.changeState(&ErrorState::instance());
                        connector.m_heartbeatTimer.expires_from_now(chrono::milliseconds(100));
                        ErrorState::instance().handleHeartbeatTimeout(connector);
                    }
                }
            }
        });
    }
    
    /////////////////////////////////////////////////
    
    ErrorState& ErrorState::instance()
    {
        static ErrorState instance;
        return instance;
    }
    
    void ErrorState::handleHeartbeatTimeout(SETPConnector &connector)
    {
        //重新连接
        //复用m_heartbeatTimer执行连接超时
        connector.m_heartbeatTimer.async_wait([this,&connector](const asio::error_code& error)
        {
            if(asio::error::operation_aborted != error)//建立连接超时
            {
                //CCLog("---------------heartbeat timeout reconnect...");
                //中断socket
                error_code err;
                connector.close(err);
                //向上层提示超时
                connector.m_onReceived(SetpError::HeartbeatTimeout(),0);
                connector._connect([this,&connector](const error_code& error)
                                   {
                                       if(error) return;//连接错误，不处理，等待超时重连
                                       connector.m_heartbeatTimer.cancel();
                                       //向上层提示超时
                                       connector.m_onReceived(SetpError::HeartbeatReconnected(),0);
                                   });
                connector.m_heartbeatTimer.expires_from_now
                    (chrono::milliseconds(s_heartbeatDuration));
                handleHeartbeatTimeout(connector);
            }
        });
    }
    
    bool ErrorState::handleReceived(SETPConnector& connector,
                                    const error_code& error,
                                    size_t bytes_transfered)
    {
        auto& handler = connector.m_onReceived;
        if(error)
        {
            if(handler) handler(error,bytes_transfered);
            return true;
        }
        
        //解析数据包
        Package pkg(connector.m_recvBuffer);
        if(pkg.getContentType()== Package::ContentType::Alert)
        {
            AlertPackage alertPkg(connector.m_recvBuffer);
            if(alertPkg.getLevel() == AlertPackage::Level::Fatal)
            {
                error_code err;
                switch(alertPkg.getCode())
                {
                    case AlertPackage::ErrorCode::UnexpectedMessage:
                    {
                        err = SetpError::UnexpectedMessage();
                        break;
                    }
                    default:
                    {
                        err = SetpError::errorCode(alertPkg.getCode());
                    }
                }
                if(connector.m_onReceived) connector.m_onReceived(err,0);
                connector.m_session.clear();
                return true;
            }
            if(alertPkg.getCode() == AlertPackage::ErrorCode::SessionFailure)
            {
                //收到session time out
                connector.m_session.clear();
                vector<unsigned char>& resendBuf = connector.m_sendBuffer;
                HandshakeHello::instance().handshake(connector,
                                                     [&connector,&handler,resendBuf]
                                                     (const error_code &err)
                {
                    if(err)
                    {
                        handler(err,0);
                        return;
                    }
                    connector.asyncWrite(&connector.m_plainSendBuf[0],
                                         connector.m_plainSendBuf.size(),
                                         [&connector,&handler](const error_code& error,
                                                      const size_t bytes_transfered)
                                         {
                                             if(error)
                                             {
                                                 handler(error,0);
                                                 return;
                                             }
                                         });
                });
                return true;
            }
            
            handler(SetpError::UnexpectedMessage(),bytes_transfered);
            return true;
        }
        handler(SetpError::UnexpectedMessage(),bytes_transfered);
        return  true;
    }
}
