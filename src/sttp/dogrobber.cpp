//
//  dogrobber.cpp
//  Smobiler
//
//  Created by jinyk on 16/2/3.
//
//

#include <stdio.h>
#include <zconf.h>
#include <zlib.h>
#include <algorithm>
#include "dogrobber.h"
#include "tinyxml2.h"

#define SOCKET_LOG_SIZE 3*1024

namespace sttp
{
    const string ContentType::STMLType = "STML";
    const string ContentType::RAWType = "RAW";
    const string _SttpPackage::s_ContentLength = "Content-Length:";
    const string _SttpPackage::s_ContentType = "Content-Type:";
    const string _SttpPackage::s_ProtocolName = "STML";
    
    
    void _SttpPackage::reset()
    {
        headLength = 0;
        contentLength = 0;
        headPos = 0;
        contentPos = 0;
        contentType = ContentType::STML;
    }
    
    Dogrobber& Dogrobber::getInstance()
    {
        static Dogrobber instance;
        return instance;
    }
    
    Dogrobber::Dogrobber() : m_isStop(true),m_force(false)
    {
        m_transceiver.setOnConnected([this](const error_code& error)
                                     {
                                         if(m_force)
                                         {
                                             m_force = false;
                                         }
                                         else
                                         {
                                         }
                                         if(m_server != m_originServer)
                                         {
                                             m_originServer = m_server;
                                         }
                                         doEventCallback(EventType::EConnected,error,nullptr);
                                     });
        
        m_transceiver.setOnDisconnected([this](const error_code& error)
                                        {
                                            doEventCallback(EventType::EDisconnect,error,nullptr);
                                        });
        
        m_transceiver.setOnRecevied(bind(&Dogrobber::handleReceived,
                                         this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));
        
        m_transceiver.setOnSended([this](const error_code& error)
                                  {
                                      doEventCallback(EventType::ESended,error,nullptr);
                                  });
        
        m_transceiver.setOnTimeout([this](const error_code& error)
                                   {
                                       doEventCallback(EventType::ETimeOut,error,nullptr);
                                   });
    }
    
    void Dogrobber::setPeer(const string& host,size_t port)
    {
        m_server = make_pair(host,port);
        m_transceiver.setPeer(m_server);
    }
    
    void Dogrobber::hello()
    {
        m_isStop = false;
        if(m_transceiver.send(""))
        {//网络连接正常情况下
//            ConnectDirective directive;
//            directive.setMode(ConnectDirective::Mode::Connect);
            if(m_force)
            {
//                directive.setMode(ConnectDirective::Mode::Reconnect);
                m_force = false;
            }
            if(m_server != m_originServer) m_originServer = m_server;
//            post(directive);
        };
    }
    
    void Dogrobber::bye()
    {
        m_isStop = true;
    }
    
    void Dogrobber::handleReceived(const unsigned char* buffer, const size_t byte_transfered)
    {
        if(m_isStop) return;
        size_t buffer_pos = 0;
        error_code errCode;
        if( 0 == m_recvPackage.headLength )
        {
            //读取head
            size_t read_len = byte_transfered > _SttpPackage::ReadAheadLength - m_recvPackage.headPos ?
            _SttpPackage::ReadAheadLength - m_recvPackage.headPos : byte_transfered;
            memcpy(&(m_recvPackage.readAheadBuf[m_recvPackage.headPos]), buffer, read_len);
            m_recvPackage.headPos += read_len;
            buffer_pos += read_len;
            
            //判断head是否读取结束
            if(find(&m_recvPackage.readAheadBuf[0],
                    &m_recvPackage.readAheadBuf[m_recvPackage.headPos],
                    '\r') == &m_recvPackage.readAheadBuf[m_recvPackage.headPos])
            {
                //m_transceiver.recv(); //未得到全部head，继续读
                return;
            }
            //分析head
            stringstream headSst;
            headSst << (char*)m_recvPackage.readAheadBuf;
            string headLine;
            size_t foundPos = 0;
            stringstream sst;
            while(!headSst.eof())
            {
                getline(headSst,headLine);
                m_recvPackage.heads.push_back(headLine);
                //查询SMTL returnCode
                foundPos = headLine.find(_SttpPackage::s_ProtocolName);
                if(foundPos!=string::npos)
                {
                    size_t foundPos1 = headLine.find_first_of(' ',foundPos);
                    size_t foundPos2 = headLine.find_first_of(' ',foundPos1 + 1);
                    sst << headLine.substr(foundPos1 + 1, foundPos2 - foundPos1 - 1);
                    sst >> m_recvPackage.returnCode;
                    sst.str("");
                    sst.clear();
                }
                //查询ContentType
                foundPos = headLine.find(_SttpPackage::s_ContentType);
                if(foundPos!=string::npos)
                {
                    sst << headLine.substr(foundPos + _SttpPackage::s_ContentType.length());
                    if(sst.str() == ContentType::RAWType)
                    {
                        m_recvPackage.contentType = ContentType::RAW;
                    }
                    sst.str("");
                    sst.clear();
                }
                //查询ContentLength
                foundPos = headLine.find(_SttpPackage::s_ContentLength);
                if(foundPos!=string::npos)
                {
                    sst << headLine.substr(foundPos + _SttpPackage::s_ContentLength.length());
                    string strr= sst.str();
                    sst >> m_recvPackage.contentLength;
                    sst.str("");
                    sst.clear();
                    if( m_recvPackage.rawData.capacity() < m_recvPackage.contentLength )
                    {
                        m_recvPackage.rawData.reserve(m_recvPackage.contentLength * 1.1);
                    }
                    m_recvPackage.rawData.resize(m_recvPackage.contentLength);
                }
                if(headLine.empty() || "\r" == headLine)
                {
                    //head结束,设置headLength，拷贝剩余部分至content缓存
                    auto gPos = headSst.tellg();
                    if( gPos < 0)
                    {
                        errCode.assign(StmlError::DataError,StmlError::instance());
                        doEventCallback(EventType::EReceived,errCode,nullptr);
                        return;
                    }
                    m_recvPackage.headLength = gPos;
                    
                    const size_t copyLen = min(m_recvPackage.headPos-m_recvPackage.headLength,
                                               m_recvPackage.contentLength);
                    memcpy(&m_recvPackage.rawData[0],
                           &m_recvPackage.readAheadBuf[m_recvPackage.headLength],
                           copyLen);
                    m_recvPackage.contentPos += m_recvPackage.headPos-m_recvPackage.headLength;
                    break;
                }
            }//end while
            
            if(0 == m_recvPackage.headLength)
            {
                errCode.assign(StmlError::DataError,StmlError::instance());
                doEventCallback(EventType::EReceived,errCode,nullptr);
                m_recvPackage.reset();
                return;
            }
        } //end if 0 == m_recvPackage.headLength
        
        //拷贝content内容
        if( m_recvPackage.contentPos < m_recvPackage.contentLength &&
           buffer_pos < byte_transfered)
        {
            const size_t copyLen = min(m_recvPackage.contentLength - m_recvPackage.contentPos,
                                       byte_transfered - buffer_pos);
            memcpy(&m_recvPackage.rawData[m_recvPackage.contentPos],
                   &buffer[buffer_pos],
                   copyLen);
            m_recvPackage.contentPos += byte_transfered - buffer_pos;
        }
        if( m_recvPackage.contentPos < m_recvPackage.contentLength)
        {
            //m_transceiver.recv();
            return;
        }
        
        if ( 200 == m_recvPackage.returnCode)
        {
            errCode.assign(0,StmlError::instance());
        }
        else
        {
            errCode.assign(m_recvPackage.returnCode,StmlError::instance());
        }
        
        //生成回调数据
        shared_ptr<SttpResponse> pResponse(new SttpResponse());
        pResponse->contentType = ContentType::STML;
        pResponse->contentLength = m_recvPackage.contentLength;
        do
        {
            if(errCode) break;
            
            unzip(m_recvPackage.content,m_recvPackage.rawData,m_recvPackage.rawData.size(),errCode);
            if(errCode)
            {
                errCode.assign(StmlError::UnzipError,StmlError::instance());
                break;
            }
            
            switch(m_recvPackage.contentType)
            {
                case ContentType::STML:
                {
                    //log recevied data
                    m_logStream.write((const char*)&m_recvPackage.content[0],
                                      SOCKET_LOG_SIZE <= m_recvPackage.content.size() ?
                                      SOCKET_LOG_SIZE : m_recvPackage.content.size());
                    m_logStream.str("");
                    
                    pResponse->xmlDoc.reset(new tinyxml2::XMLDocument());
                    
                    tinyxml2::XMLError error =  pResponse->xmlDoc->Parse
                    (
                        (const char*)(&m_recvPackage.content[0]),
                        m_recvPackage.content.size()
                    );
                    
                    if(error != tinyxml2::XML_SUCCESS)
                    {
                        errCode.assign(StmlError::ParseError,StmlError::instance());
                    }
                    break;
                }
                default:
                {
                    m_recvPackage.reset();
                    return;
                }
            }
        } while(0);
        doEventCallback(EventType::EReceived,errCode,pResponse);
        
        m_recvPackage.reset();
    }
    
    //发送指令
    //immediate : 立即发送。发送时将指令移至最前。
//    void Dogrobber::post(const Directive &directive,bool immediate)
//    {
//        string msg = directive.str();
//        m_transceiver.send(msg,immediate);
//    }
//    
    void Dogrobber::post(const string &msg)
    {
        m_transceiver.send(msg);
    }
    
    void Dogrobber::unzip(vector<unsigned char>& record,
                          const vector<unsigned char>& inBuf,
                          size_t len,
                          error_code& errCode)
    {
        static const string head("<STML>");
        memcpy(&record[0], head.c_str(), head.size());
        
        int err;
        z_stream strm;
        strm.next_in = (Byte *)&inBuf[0];
        strm.avail_in = (unsigned int)len;
        strm.total_out = 0;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        err = inflateInit2(&strm,47);
        if(err!=Z_OK)
        {
            errCode.assign(StmlError::UnzipError,StmlError::instance());
            return;
        }
        
        while (true)
        {
            // Make sure we have enough room and reset the lengths.
            size_t data_len = record.size()-head.size();
            if (strm.total_out >= data_len)
            {
                if(record.capacity() < data_len) record.reserve(data_len * 1.5);
                record.resize(data_len * 1.5);
            }
            strm.next_out = &record[head.size() + strm.total_out];
            strm.avail_out = (unsigned int)(record.size() - head.size() - strm.total_out);
            
            // Inflate a chunk.
            err = inflate (&strm, Z_SYNC_FLUSH);
            if (err == Z_STREAM_END) break;
            if (err != Z_OK)
            {
                errCode.assign(StmlError::UnzipError,StmlError::instance());
                return;
            };
        }
        
        static const string tail("</STML>0");
        record.resize(strm.total_out + head.size() + tail.size());
        memcpy(&record[strm.total_out+head.size()], tail.c_str(), tail.size());
        record[record.size()-1] = 0;
        err = inflateEnd(&strm);
        if(err!=Z_OK)
        {
            errCode.assign(StmlError::UnzipError,StmlError::instance());
        }
    }
    
    void Dogrobber::doEventCallback(const EventType type,
                                    const error_code& err,
                                    shared_ptr<SttpResponse> pResponse)
    {
        if(m_eventCallback)
        {
//            CAScheduler::getScheduler()->dispatch(bind(m_eventCallback,type,err,pResponse));
            m_eventCallback(type, err, pResponse);
        }
    }
    
}//end namespace sttp
