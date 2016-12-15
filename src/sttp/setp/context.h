//
//  setp_stream.h
//  Smobiler
//
//  Created by jinyk on 16/2/26.
//
//

#ifndef __setp_stream_h
#define __setp_stream_h

#include <vector>
#include <cryptopp/rsa.h>
#include "asio.hpp"

using namespace std;
using namespace asio;
using namespace CryptoPP;

namespace setp
{
    
    typedef function<void(const error_code& error,const size_t)> _IoHandler;
    typedef function<void(const error_code& error)> _Handler;
    
    class Serializer
    {
    public:
        static uint32_t getUint32(unsigned char* p);
        static void set(unsigned char * p,uint32_t i);
        
    private:
        static void reverse(unsigned char* from,unsigned char * to,size_t length);
        static bool m_islittleEndian;
    };

    
    class Session
    {
    public:
        enum
        {
            PublicKeyLength = 128,
            AesKeyLength = 32,
            AesIvLength = 16,
            SessionIdLength = 16,
            RandomKeyLength = 4,
        };
        
        void setRandomKey1(uint32_t key)
        {
            Serializer::set(&m_randomKey[0],key);
        }
        
        void setRandomKey2(unsigned char* key)
        {
            memcpy(&m_randomKey[4],key,4);
        }
        
        void setRandomKey3(uint32_t key)
        {
            Serializer::set(&m_randomKey[8],key);
        }
        
        unsigned char* getRandomKey3()
        {
            return &m_randomKey[8];
        }
        
        unsigned char* getRandomKey()
        {
            return m_randomKey;
        }
        
        unsigned char* getAesKey()
        {
            return m_aesKey;
        }
        
        unsigned char* getAesIv()
        {
            return m_aesIv;
        }
        
        unsigned char* getSessionId()
        {
            return m_sessionId;
        }
        
        void setVersion(unsigned char major,unsigned char minor)
        {
            m_serverMajorVersion = major;
            m_serverMinorVersion = minor;
        }
        
        const RSAES_PKCS1v15_Encryptor& getPublicKey() const
        {
            return m_pubKey;
        }
        
        void setPublicKey(ArraySource& keybuf)
        {
            m_pubKey = RSAES_PKCS1v15_Encryptor(keybuf);
        }
        
        void clear();
        
    private:
        unsigned char m_randomKey[RandomKeyLength*3];
        unsigned char m_aesIv[AesIvLength];
        unsigned char m_aesKey[AesKeyLength];
        unsigned char m_serverMajorVersion;
        unsigned char m_serverMinorVersion;
        unsigned char m_sessionId[SessionIdLength];
        RSAES_PKCS1v15_Encryptor m_pubKey;
    };
    
    class Package
    {
    public:
        enum class ContentType
        {
            Alert = 21,
            Handshake = 22,
            ApplicationData = 23,
            Heartbeat = 24
        };
        
        enum { head_length = 8 };
        
    public:
        Package(vector<unsigned char>& package) : m_package(package){};
        
        unsigned char* getBuffer()
        {
            return &m_package[0];
        }
        
        size_t getBufferSize() const
        {
            return m_package.size();
        }
        
        uint32_t getLength() const
        {
            return Serializer::getUint32(&m_package[4]);
        }
        
        ContentType getContentType() const
        {
            return (ContentType)m_package[0];
        }
        
        void setLength(size_t i)
        {
            Serializer::set(&m_package[4],(uint32_t)i);
        }
        
        void asApplicationData()
        {
            setContentType(ContentType::ApplicationData);
        }
        
    protected:
        void setContentType(ContentType type)
        {
            m_package[0] = (unsigned char)type;
        }
    
    protected:
        vector<unsigned char>& m_package;
    };
    
    class AlertPackage : public Package
    {
    public:
        enum class Level
        {
            Warning = 1,
            Fatal = 2
        };
        
        enum class ErrorCode
        {
            UnexpectedMessage 	= 10,
            HandshakeFailure 	= 40,
            SessionFailure 	= 48
        };
    public:
        AlertPackage(vector<unsigned char>& package) : Package(package)
        {};
        
        Level getLevel() const
        {
            return (Level)m_package[8];
        }
        
        ErrorCode getCode()
        {
            return (ErrorCode)m_package[9];
        }
    };
    
    class HeartbeatPackage : public Package
    {
    public:
        enum class Type
        {
            Request = 1,
            Echo = 2
        };
        
    public:
        HeartbeatPackage(vector<unsigned char>& package) : Package(package)
        {};
        
        Type getType() const
        {
            return (Type)m_package[8];
        }
        
        void setType(Type type)
        {
            m_package[8] = (unsigned char)type;
        }
        
        uint32_t getData()
        {
            return Serializer::getUint32(&m_package[9]);
        }
        
        void setData(uint32_t d)
        {
            Serializer::set(&m_package[9], d);
        }
        
        void asClient();
    };

    
    class HandshakePackage : public Package
    {
    public:
        enum class MessageType
        {
            ClientHello = 1,
            ServerHello = 2,
            ClientConfirm = 3,
            ServerConfirm = 4
        };
    public:
        HandshakePackage(vector<unsigned char>& package) : Package(package)
        {}
        
        MessageType getMessageType()  const
        {
            return (MessageType)m_package[8];
        }
        
    protected:
        void setMessageType(MessageType type)
        {
            m_package[8] = (unsigned char)type;
        }
    };
    
    class HandshakeHelloPackage : public HandshakePackage
    {
    public:
        HandshakeHelloPackage(vector<unsigned char>& package) : HandshakePackage(package)
        {}
        
        unsigned char* getRandom()
        {
            return &m_package[12];
        }
        
        void setRandom(uint32_t i)
        {
            Serializer::set(&m_package[12],i);
        }
        
        unsigned char* getSessionId()
        {
            return &m_package[16];
        }
        
        unsigned char getMajorVersion()
        {
            return m_package[32];
        }
        
        void setMajorVersion(unsigned char b)
        {
            m_package[32] = b;
        }
        
        unsigned char getMinorVersion() const
        {
            return m_package[33];
        }
        
        void setMinorVersion(unsigned char b)
        {
            m_package[33] = b;
        }
        
        unsigned char* getRsaKey()
        {
            return &m_package[36];
        }
        
        size_t getRsaKeyLength() const
        {
            return getLength() - 28;
        }
        
        void asClient();
    };
    
    class HandshakeConfirmPackage : public HandshakePackage
    {
    public:
        HandshakeConfirmPackage(vector<unsigned char>& package) : HandshakePackage(package)
        {}
        
        unsigned char* getPremasteKey()
        {
            return &m_package[12];
        }
        
        void asClient();
    };
    
    
    class SetpError : public error_category
    {
    public:
        enum class Error
        {
            Unknown = 200,
            UnexpectedMessage = 201,
            PubKeyError = 202,
            SessionFailure = 203,
            UnknowndMessage = 204,
            EncryptionError = 205,
            DecryptionError = 206,
            HeartbeatTimeout = 207,
            HeartbeatReconnected = 208, //因心跳超时而重连成功
        };
        
    public:
        static SetpError& instance();
        
        static error_code errorCode(AlertPackage::ErrorCode code);
        
        static error_code UnexpectedMessage()
        {
            return error_code((int)Error::UnexpectedMessage, SetpError::instance());
        }
        
        static error_code UnknownMessage()
        {
            return error_code((int)Error::UnknowndMessage, SetpError::instance());
        }
        
        static error_code PubKeyError()
        {
            return error_code((int)Error::PubKeyError, SetpError::instance());
        }
        
        static error_code EncryptionError()
        {
            return error_code((int)Error::EncryptionError, SetpError::instance());
        }
        
        static error_code DecryptionError()
        {
            return error_code((int)Error::DecryptionError, SetpError::instance());
        }

        static error_code HeartbeatTimeout()
        {
            return  error_code((int)Error::HeartbeatTimeout, SetpError::instance());
        }
        
        static error_code HeartbeatReconnected()
        {
            return  error_code((int)Error::HeartbeatReconnected, SetpError::instance());
        }
        
        virtual const char* name() const noexcept
        {
            return "setp_error";
        }
        
        virtual bool equivalent (const error_code& code, int condition) const noexcept
        {
            return *this==code.category() &&
            static_cast<int>(default_error_condition(code.value()).value())==condition;
        }
        
        virtual string message(int ev) const;
        
    private:
        SetpError(){};
    };
    
}//end namespace setp


#endif /* setp_stream_h */
