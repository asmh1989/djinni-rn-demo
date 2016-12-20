//
//  dogrobber.h
//  Smobiler
//
//  Created by jinyk on 16/2/3.
//
//

#ifndef dogrobber_h
#define dogrobber_h

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <memory>
#include "tinyxml2.h"
#include "transceiver.h"

using namespace std;

namespace sttp
{
    struct ContentType
    {
        enum Type
        {
            STML = 0,
            RAW
        };
        static const string STMLType;
        static const string RAWType;
    };
    
    class Dogrobber;
    
    class _SttpPackage
    {
        friend class Dogrobber;
    public:
        _SttpPackage() : headLength(0),
        contentLength(0),
        headPos(0),
        contentPos(0),
        returnCode(0),
        content(8 * 1024)
        {
            memset(readAheadBuf, 0, ReadAheadLength + 1);
        }
        
        vector<unsigned char> rawData;
        vector<unsigned char> content;
        vector<string> heads;
        
        size_t headLength;
        size_t contentLength;
        uint32_t returnCode;
        ContentType::Type contentType = ContentType::Type::STML;
        
    private:
        void reset();
        
    private:
        static const string s_ContentLength;
        static const string s_ContentType;
        static const string s_ProtocolName;
        enum { ReadAheadLength = 128 };
        
        unsigned char readAheadBuf[ReadAheadLength + 1];
        size_t headPos;
        size_t contentPos;
        
    };
    
    struct SttpResponse
    {
        unique_ptr<tinyxml2::XMLDocument> xmlDoc;
        unique_ptr<unsigned char[]> rawData;
        ContentType::Type contentType;
        size_t contentLength = 0;
    };
    
    class Dogrobber
    {
    public:
        enum class EventType
        {
            EConnected = 0,
            EReconnected,
            EDisconnect,
            ESended,
            EReceived,
            ETimeOut,
            EUnknown
        };
        
        typedef function<void(const EventType,const error_code&,shared_ptr<SttpResponse>)> EventCallback;
        
    public:
        static Dogrobber& getInstance();
        
        void setPeer(const string& host,size_t port);
        
        void hello();
        
        void bye();
        
        void post(const string& msg);
                
        void setEventCallback(EventCallback&& callback)
        {
            m_eventCallback = callback;
        }
        
        void forceReconnect()
        {
            m_force = true;
        }
        
        void setKeepAlive(bool keepAlive)
        {
            m_transceiver.setKeepAlive(keepAlive);
        }
        
        bool isConnected();
        
    protected:
        Dogrobber();
        
    private:
        pair<string,size_t> m_server;
        pair<string,size_t> m_originServer;
        
        void handleReceived(const unsigned char* buffer, const size_t len);
        void unzip(vector<unsigned char>& record,
                   const vector<unsigned char>& inBuf,
                   size_t len,
                   error_code& error);
        
        void doEventCallback(const EventType,const error_code&,shared_ptr<SttpResponse>);
        
    private:
        typedef SETPTransceiver Transceiver;
        //typedef TCPTransceiver Transceiver;
        Transceiver m_transceiver;
        
        EventCallback m_eventCallback;
        
        _SttpPackage m_recvPackage;
        
        stringstream m_logStream;
        
        EventType m_connectType;
        
        atomic<bool> m_isStop;
        atomic<bool> m_force;
    };
    
}//end namespace sttp

#endif /* dogrobber_h */
