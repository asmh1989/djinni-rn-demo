//
//  error.cpp
//  Smobiler
//
//  Created by jinyk on 16/3/3.
//
//

#include <stdio.h>
#include "error.h"
#include "setp/context.h"

namespace sttp
{
    SttpError& SttpError::instance()
    {
        static SttpError instance;
        return instance;
    }
    
    string SttpError::message(int ev) const
    {
        switch(ev)
        {
            case (int)0:
            {
                return "Ok.";
            }
            case (int)errc::operation_not_permitted:
            {
                return "操作被拒绝 请检查服务器地址设置";
            }
            case (int)errc::connection_refused:
            {
                return "网络连接被拒绝";
            }
            case (int)errc::connection_reset:
            {
                return "网络连接被重置";
            }
            case (int)errc::network_unreachable:
            {
                return "无法连接到网络";
            }
            case (int)errc::network_down:
            {
                return "没有连接到网络";
            }
            case (int)errc::timed_out:
            {
                return "网络无响应";
            }
            case (int)errc::broken_pipe:
            {
                return "网络已断开，请重试";
            }
            case(int)errc::not_connected:
            {
                return "网络已断开，请重试";
            }
            case (int)errc::no_child_process:
            {
                return "无法处理";
            }
            case (int)setp::SetpError::Error::UnexpectedMessage:
            {
                 return "网络不给力，请重试！";
            }
            case (int)setp::SetpError::Error::UnknowndMessage:
            {
                return "收到非法数据";
            }
            case (int)setp::SetpError::Error::PubKeyError:
            {
                return "解码错误";
            }
            case (int)setp::SetpError::Error::SessionFailure:
            {
                return "会话超时";
            }
            case (int)setp::SetpError::Error::HeartbeatTimeout:
            {
                return "网络不给力，努力恢复中...";
            }
            default:
            {
                return "";
            }
        }
    }
    
    StmlError& StmlError::instance()
    {
        static StmlError instance;
        return instance;
    }
    
    std::string StmlError::message(int ev) const {
        switch (ev)
        {
            case 0: return "Ok";
            case Ok: return "OK";
            case ServerError: return "服务器异常";
            case DataError: return "返回数据异常";
            case UnzipError : return "解压缩数据异常";
            case ParseError : return "解析数据异常";
            default: return "";
        }
    }
    
}//end namespace
