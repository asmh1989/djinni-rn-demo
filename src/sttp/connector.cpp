//
//  connector.cpp
//  Smobiler
//
//  Created by jinyk on 16/2/25.
//
//

#include <stdio.h>
#include <iostream>
#include "connector.h"

namespace sttp
{
    
    void _TCPConnector::connect(const string & host,
                                std::size_t port,
                                const function<void(const error_code& error)>& callback)
    {
        stringstream sst;
        sst << port;
        ip::tcp::resolver resolver(m_ioService);
        asio::async_connect(m_stream,
                            resolver.resolve({ host, sst.str()}),
                            [callback](std::error_code err, ip::tcp::resolver::iterator)
                            {
                                callback(err);
                            });
    }
    
} // end namespace sttp