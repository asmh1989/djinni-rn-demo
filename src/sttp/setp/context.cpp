//
//  setp_stream.cpp
//  Smobiler
//
//  Created by jinyk on 16/2/26.
//
//

#include <stdio.h>
#include "context.h"

namespace setp
{
    error_code SetpError::errorCode(AlertPackage::ErrorCode code)
    {/*
        Error err;
        switch(code)
        {
            case AlertPackage::ErrorCode::UnexpectedMessage :
            {
                err = Error::UnexpectedMessage;
                break;
            }
            case AlertPackage::ErrorCode::SessionFailure:
            {
                err = Error::SessionFailure;
                break;
            }
            default :
            {
                err = Error::Unknown;
            }
                
        }
      */
        error_code ec((int)code, SetpError::instance());
        return ec;
    }
    
    SetpError& SetpError::instance()
    {
        static SetpError instance;
        return instance;
    }
    
    string SetpError::message(int ev) const
    {
        switch(ev)
        {
                
        }
        return "";
    }
    
    static bool littleEndian()
    {
        union w
        {
            int  a;
            char b[sizeof(int)];
        } c;
        c.a = 1;
        return(c.b[0] == 1);
    }
    
    bool Serializer::m_islittleEndian = littleEndian();
    
    void Serializer::reverse(unsigned char* to,unsigned char * from,size_t length)
    {
        for(int i = 0; i < length; ++i)
            to[i] = from[length - i - 1];
    }

    uint32_t Serializer::getUint32(unsigned char *p)
    {
        if(Serializer::m_islittleEndian)
        {
            return *reinterpret_cast<uint32_t*>(p);
        }
        
        union
        {
            uint32_t a;
            unsigned char b[4];
        } result;
        Serializer::reverse(&result.b[0], p, 4);
        return result.a;
    }
    
    void Serializer::set(unsigned char * p,uint32_t i)
    {
        if(Serializer::m_islittleEndian)
        {
            *reinterpret_cast<uint32_t*>(p) = i;
            return;
        }
        
        union
        {
            uint32_t a;
            unsigned char b[4];
        } value;
        value.a = i;
        Serializer::reverse(p, &value.b[0], 4);
    }
    
    void Session::clear()
    {
        memset(m_randomKey,0,RandomKeyLength*3);
        memset(m_aesIv,0,AesIvLength);
        memset(m_aesKey,0,AesKeyLength);
        memset(m_sessionId,0,SessionIdLength);
    }
    
    void HandshakeHelloPackage::asClient()
    {
        m_package.resize(36);
        setLength(36-8);
        setContentType(ContentType::Handshake);
        setMessageType(HandshakePackage::MessageType::ClientHello);
        memset(&m_package[16],0,16);
    }
    
    void HandshakeConfirmPackage::asClient()
    {
        m_package.resize(Session::PublicKeyLength + 12);
        setLength(Session::PublicKeyLength + 12 - 8);
        setContentType(ContentType::Handshake);
        setMessageType(HandshakePackage::MessageType::ClientConfirm);
    }
    
    void HeartbeatPackage::asClient()
    {
        m_package.resize(13);
        setLength(5);
        setContentType(ContentType::Heartbeat);
        setType(HeartbeatPackage::Type::Request);
    }
    
}//end namespace setp
