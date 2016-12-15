//
//  comm_channel.h
//  Smobiler
//
//  Created by jinyk on 16/1/28.
//
//

#ifndef __comm_channel__
#define __comm_channel__

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <mutex>
#include "asio.hpp"
#include "asio/deadline_timer.hpp"
//#include "asio/ssl.hpp"

#include "connector.h"
#include "setp/connector.h"
#include "error.h"
#include "log_impl.hpp"

using namespace asio;
using namespace std;
using namespace smobiler;

namespace sttp
{

typedef function<void(const error_code&)> OnConnectedEvent;
typedef function<void(const error_code&)> OnDisconnectedEvent;
typedef function<void(const error_code&)> OnTimeoutEvent;
typedef function<void(const error_code&)> OnSendedEvent;
typedef function<void(const unsigned char*,size_t)> OnReceivedEvent;
typedef function<void(const error_code&)> OnDisconnectedEvent;

    
template<class Connector>
class _Transceiver
{
public:
    enum class State
    {
        Disconnect,  // 未连接
        Connecting,  // 正在连接
        Connected,   // 已经连接
    };
    
private:
    enum class WaitType
    {
        Response,
        Connect,
        Send
    };
    
public:
    _Transceiver() : m_connector(m_ioService),
                     m_worker(m_ioService),
                     m_timer(m_ioService),
                     m_state(State::Disconnect),
                     m_isSending(false)
    {
        std::thread([this](){
            asio::error_code error;
            //只有一个网络线程（收发数据按单线程设计）
            m_ioService.run(error);
            if(error)
            {
                LogImpl::e(TAG_ERROR, error.message());
            }
        }).detach();
    }
    
    ~_Transceiver()
    {
        disconnect(asio::error::operation_aborted);
        m_ioService.stop();
        std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
    
    void setOnTimeout(const OnTimeoutEvent& callback)
    {
        m_onTimeout = callback;
    }
    
    void setOnConnected(const OnConnectedEvent& callback)
    {
        m_onConnected = callback;
    }
    
    void setOnDisconnected(const OnDisconnectedEvent& callback)
    {
        m_onDisconnected = callback;
    }
    
    void setOnRecevied(const OnReceivedEvent& callback)
    {
        m_onReceived = callback;
    }
    
    void setOnSended(const OnSendedEvent& callback)
    {
        m_onSended = callback;
    }
    
    State getState() const
    {
        return m_state;
    }
    
    void setPeer(const pair<string,size_t>& peer)
    {
        if(peer != m_peer && (State::Connected == m_state ||
                              State::Connecting == m_state) )
        {
            error_code error(asio::error::connection_aborted);
            disconnect(error);
        }
        m_peer = peer;
    }
    
    void disconnect(const error_code& error)
    {
        LogImpl::d(TAG_SOCKET, "Transceiver disconnected .");
        m_timer.cancel();
        m_isSending = false;
        error_code close_err;
        m_connector.close(close_err);
        m_state = State::Disconnect;
        
        m_ioService.post([this](){
            m_sendBuffer.clear();
        });
        
        if(error == asio::error::operation_aborted ||
           error == asio::error::connection_aborted ) return;
        
        error_code sttpError(error.value(),SttpError::instance());
        if(m_onDisconnected) m_onDisconnected(sttpError);
    }
    
    //发送数据
    //return: 连接断开的情况下，返回false，否者返回true。
    bool send(const string &str,bool immediate = false)
    {
        LogImpl::d(TAG_SOCKET, "Transceiver request send: ");
        m_ioService.post([this,str,immediate]()
        {
            if(!str.empty())
            {
                if(immediate)
                {
                    m_sendBuffer.push_front(str);
                }
                else
                {
                    m_sendBuffer.push_back(str);
                }
            }
        });
        
        if(State::Connected != m_state)
        {
            connect();
            return false;
        }
        m_ioService.post([this]()
                         {
                             if(!m_isSending) flush();
                         });
        
        return true;
    }
    
    void recv()
    {
        LogImpl::d(TAG_SOCKET, "Transceiver request recv.");
        m_connector.asyncRead([this](const error_code& error,const size_t bytes_transfered)
                              {
                                  m_timer.cancel();
                                  if (error)
                                  {
                                      stringstream sst;
                                      sst << "Transceiver read err:" << error.message()
                                      << ", errorcode=" << error.value();
                                      LogImpl::e(TAG_ERROR, sst.str());
                                      error_code resultErr = error;
                                      switch (resultErr.value())
                                      {
                                          case asio::error::eof:
                                          {
                                              resultErr = SttpError::ConnectionReset();
                                              break;
                                          }
                                          case (int)setp::SetpError::Error::HeartbeatReconnected:
                                          {
                                              onConnected(resultErr);
                                              return;
                                          }
                                          default:
                                          {
                                              break;
                                          }
                                      }
                                      if(resultErr.value() != asio::error::operation_aborted)
                                      {
                                          disconnect(resultErr);
                                      }
                                      return;
                                  }
                                  if( 0 == bytes_transfered )
                                  {
                                      disconnect(error_code((int)errc::broken_pipe,SttpError::instance()));
                                      return;
                                  }
                                  
                                  LogImpl::d(TAG_SOCKET, "Transceiver received ");
                                  if (m_onReceived)
                                  {
                                      m_onReceived(m_connector.getRecvData(),
                                                   bytes_transfered);
                                  }
                              });
    }
    
    void setKeepAlive(bool keepAlive)
    {
        m_connector.setKeepAlive(keepAlive);
    }
    
private:
    void connect()
    {
        if(State::Connected == m_state) return;
        
        m_state = State::Connecting;
        m_connector.connect(m_peer.first,m_peer.second,
                            [this](const error_code& error)
                            {
                                m_timer.cancel();
                                if (error)
                                {
                                    LogImpl::e(TAG_ERROR, "Transceiver async_connect error:" + error.message());
                                    m_state = State::Disconnect;
                                    if(error.value() == asio::error::operation_aborted) return;
                                    error_code sttpError(error.value(),SttpError::instance());
                                    if(m_onDisconnected) m_onDisconnected(sttpError);
                                    return;
                                }
                                
                                onConnected(error);
                            });
        
        startTimer(WaitType::Connect);
        LogImpl::d(TAG_SOCKET, "Transceiver connecting ... ");
    }
    
    void onConnected(const error_code& error)
    {
        m_state = State::Connected;
        recv();
        
        error_code sttpError(error.value(),SttpError::instance());
        if (m_onConnected) m_onConnected(sttpError);
        
        LogImpl::d(TAG_SOCKET, "Transceiver async_connect success.");
        
        
        //if(!m_isSending) flush();
    }
    
    bool flush()
    {
        if(m_sendBuffer.empty())
        {
            m_isSending = false;
            return false;
        }
        m_isSending = true;
        LogImpl::d(TAG_SOCKET, "Transceiver send:" + m_sendBuffer.front());
        m_connector.asyncWrite((unsigned char*)m_sendBuffer.front().c_str(),
                               m_sendBuffer.front().length(),
                               [this](const error_code& error,size_t bytes_transfered)
                               {
                                   m_timer.cancel();
                                   if (error)
                                   {
                                       m_isSending = false;
                                       disconnect(error);
                                       LogImpl::e(TAG_ERROR, "Transceiver Send failed: " + error.message());
                                       return;
                                   }
                                   
                                   error_code sttpError(error.value(),SttpError::instance());
                                   if(m_onSended) m_onSended(sttpError);
                                   m_sendBuffer.pop_front();
                                   
                                   flush();
                                   startTimer(WaitType::Response);
                               });
        
        startTimer(WaitType::Send);
        return true;
    }
    
    void handleTimeout(WaitType type, const asio::error_code& error)
    {
        if(error.value() == asio::error::operation_aborted) return;
        LogImpl::d(TAG_SOCKET, "Transceiver Timeout: " + error.message());
        
        switch(type)
        {
            case WaitType::Response:
            {
                disconnect(asio::error::timed_out);
                break;
            }
            case WaitType::Connect:
            {
                disconnect(asio::error::timed_out);
                break;
            }
            case WaitType::Send:
            {
                disconnect(asio::error::timed_out);
                break;
            }
            default:
            {
                disconnect(asio::error::connection_aborted);
                error_code sttpError(error.value(),SttpError::instance());
                if(m_onTimeout) m_onTimeout(sttpError);
            }
        }
    }
    
    void startTimer(WaitType type)
    {
        m_timer.expires_from_now(std::chrono::seconds(30));
        m_timer.async_wait(bind(&_Transceiver<Connector>::handleTimeout,
                                this,
                                type,
                                std::placeholders::_1));
    }

private:
    OnTimeoutEvent m_onTimeout;
    OnConnectedEvent m_onConnected;
    OnReceivedEvent m_onReceived;
    OnDisconnectedEvent m_onDisconnected;
    OnSendedEvent m_onSended;
    
    asio::io_service m_ioService;
    Connector m_connector;
    asio::io_service::work m_worker;
    asio::c11_deadline_timer m_timer;
    
    pair<string,size_t> m_peer;
    
    atomic<State> m_state;
    
    deque<string> m_sendBuffer;
    bool m_isSending;
};

typedef _Transceiver<_TCPConnector> TCPTransceiver;
typedef _Transceiver<_SSLConnector> SSLTransceiver;
typedef _Transceiver<setp::SETPConnector> SETPTransceiver;

} // end namespace sttp

#endif
