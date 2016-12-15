//
//  connector.h
//  Smobiler
//
//  Created by jinyk on 16/2/25.
//
//

#ifndef __connector_h
#define __connector_h

#include "asio.hpp"

using namespace asio;
using namespace std;

namespace sttp
{
    
    typedef ip::tcp::socket TCPSocket;
    typedef ip::tcp::socket SSLSocket;
    //typedef ssl::stream<ip::tcp::socket> SSLSocket;
    
    class _TCPConnector
    {
    public:
        typedef function<void(const error_code& error,const size_t)> ReceiveHandler;
        typedef function<void(const error_code& error,const size_t)> SendHandler;
        
    public:
        _TCPConnector(asio::io_service& ioService)
        : m_ioService(ioService),
        m_stream(ioService)
        {}
        
        void connect(const string & host,
                     std::size_t port,
                     const function<void(const error_code& error)>& callback);
        
        void close(error_code& error)
        {
            m_stream.close(error);
        }
        
        void asyncWrite(const unsigned char* pBuf,size_t size,
                        const function<void(const error_code&,size_t)>& handler)
        {
            return async_write(m_stream,buffer(pBuf,size),handler);
        }
        
        void asyncRead(const function<void(const error_code&,size_t)>& handler)
        {
            m_stream.async_read_some(buffer(m_recvBuffer,max_length),handler);
        }
        
        unsigned char *getRecvBuffer()
        {
            return m_recvBuffer;
        }
        
        unsigned char *getRecvData()
        {
            return &m_recvBuffer[0];
        }
        
    private:
        asio::io_service& m_ioService;
        TCPSocket m_stream;
        
        enum { max_length = 4096 };
        unsigned char m_recvBuffer[max_length];
    };
    
    
    class _SSLConnector
    {
    public:
        _SSLConnector(asio::io_service& ioService)
        : m_ioService(ioService),
        //m_contex(ssl::context::sslv3),
        m_stream(ioService)
        {}
        
        void connect(const string & host,
                     std::size_t port,
                     const function<void(const error_code& error)>& callback);
        
        SSLSocket& getStream()
        {
            return m_stream;
        }
        
    private:
        void handleConnect(const error_code& error);
        
        void handleHandshake(const error_code& error);
        
    private:
        function<void(const error_code& error)> m_callback;
        asio::io_service& m_ioService;
        //ssl::context m_contex;
        SSLSocket m_stream;
    };
    
}//end namespace sttp

#endif /* connector_h */
