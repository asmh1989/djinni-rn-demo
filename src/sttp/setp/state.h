//
//  state.hpp
//  Smobiler
//
//  Created by jinyk on 16/11/25.
//
//

#ifndef state_hpp
#define state_hpp

#include <stdio.h>
namespace setp
{
    class SETPConnector;
    
    class State
    {
    public:
        
        virtual ~State()
        {}
        
        virtual bool handleReceived(SETPConnector& connector,
                                    const error_code& error,
                                    size_t bytes_transfered) = 0;
        
        virtual void setKeepAlive(SETPConnector& connector,bool){};
        
    protected:
        _IoHandler m_handler;
    };
    
    class HandshakeHello : public State
    {
    public:
        static HandshakeHello& instance();
        
        void handshake(SETPConnector& connector,
                       const _Handler& handler);
        
        bool handleReceived(SETPConnector& connector,
                            const error_code& error,
                            size_t bytes_transfered) override;
        
    private:
        vector<unsigned char> m_sendBuffer;
    };
    
    class HandshakeConfirm : public State
    {
    public:
        static HandshakeConfirm& instance();
        
        void handshake(SETPConnector& connector,
                       const _Handler& handler);
        
        bool handleReceived(SETPConnector& connector,
                            const error_code& error,
                            size_t bytes_transfered) override;
        
    private:
        vector<unsigned char> m_sendBuffer;
        
    };
    
    class HandshakeFinish : public State
    {
    public:
        static HandshakeFinish& instance();
        
        void handshake(SETPConnector& connector,
                       const _Handler& handler);
        
        bool handleReceived(SETPConnector& connector,
                            const error_code& error,
                            size_t bytes_transfered) override;
        
        virtual void setKeepAlive(SETPConnector& connector,bool keepAlive) override;
        
        void startHeartbeat(SETPConnector& connector);
        
    private:
        vector<unsigned char> m_sendBuffer;
        bool m_heartbeatEcho = false;
        bool m_keepAlive = false;
        uint32_t m_heartbeatNum = 0;
    };
    
    class ErrorState : public State
    {
    public:
        static ErrorState& instance();
        
        void handleHeartbeatTimeout(SETPConnector& connector);
        
        bool handleReceived(SETPConnector& connector,
                            const error_code& error,
                            size_t bytes_transfered) override;
    };
}
#endif /* state_hpp */
