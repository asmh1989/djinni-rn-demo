//
//  connector.h
//  Smobiler
//
//  Created by jinyk on 16/2/29.
//
//

#ifndef __setp_connector_h
#define __setp_connector_h

#include <functional>
#include "asio.hpp"
#include "context.h"

using namespace std;

namespace setp
{
    class ConnectCondition
    {
    public:
        ConnectCondition(asio::io_service& io_service,const string& port)
        :m_ioService(io_service),m_port(port)
        {}
        
        ip::tcp::resolver::iterator operator()(const error_code& ec, const ip::tcp::resolver::iterator& iter);
        
    private:
        asio::io_service& m_ioService;
        string m_port;
    };
    
    class State;
    
    class SETPConnector
    {
        friend class HandshakeHello;
        friend class HandshakeConfirm;
        friend class HandshakeFinish;
        friend class ErrorState;
    public:
        typedef _IoHandler IoHandler;
        typedef _Handler Handler;
        
    public:
        SETPConnector(asio::io_service& ioService);
        
        void connect(const string & host,
                     std::size_t port,
                     const Handler& handler);
        
        void close(error_code& error);
        
        Session& getSession()
        {
            return m_session;
        }
        
        void asyncWrite(const unsigned char* pBuf,
                        const size_t size,
                        const IoHandler&& handler);
        
        void asyncRead(const IoHandler& handler);
        
        void changeState(State* pState);
        
        unsigned char* getRecvBuffer()
        {
            return &m_recvBuffer[0];
        }
        
        unsigned char* getRecvData()
        {
            return &m_recvBuffer[Package::head_length];
        }
        
        void setKeepAlive(bool keepAlive);
        
    private:
        void _asyncRead();
        void _connect(const Handler& handler);
        
    private:
        asio::io_service& m_ioService;
        ip::tcp::socket m_stream;
        asio::c11_deadline_timer m_heartbeatTimer;
        
        vector<unsigned char> m_recvBuffer;
        vector<unsigned char> m_sendBuffer;
        vector<unsigned char> m_plainSendBuf;

        State* m_pState = nullptr;
        Session m_session;
        
        IoHandler m_onReceived;
        
        bool m_keepAlive = false;
        bool m_stopRead = false;
        
        string  m_host;
        size_t  m_port = 0;
        
    };
    
}//end namespace setp


#endif /* connector_h */
