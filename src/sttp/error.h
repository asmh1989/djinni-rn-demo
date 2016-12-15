//
//  error.h
//  Smobiler
//
//  Created by jinyk on 16/3/3.
//
//

#ifndef _sttp_error_h
#define _sttp_error_h

#include <string>
#include <system_error>

using namespace std;

namespace sttp
{
    
    class SttpError : public error_category
    {
    public:
        static SttpError& instance();
        
        enum Error
        {
            
        };
        
    public:
        static error_code ConnectionReset()
        {
            return error_code((int)errc::connection_reset,SttpError::instance());
        }
        
        virtual const char* name() const noexcept
        {
            return "sttp_error";
        }
        
        virtual bool equivalent (const error_code& code, int condition) const noexcept
        {
            return *this==code.category() &&
            static_cast<int>(default_error_condition(code.value()).value())==condition;
        }
        
        virtual string message(int ev) const;
        
    private:
        SttpError()
        {};
    };
    
    class StmlError : public std::error_category
    {
    public:
        enum Error
        {
            Ok = 200,
            ServerError = 404,
            DataError = 501,
            ParseError = 502,
            UnzipError = 503
            
        };
        
    public:
        static StmlError& instance();
        
        virtual const char* name() const noexcept
        {
            return "stml_error";
        }
        
        virtual bool equivalent (const std::error_code& code, int condition) const noexcept
        {
            return *this==code.category() &&
            static_cast<int>(default_error_condition(code.value()).value())==condition;
        }
        
        virtual std::string message(int ev) const;
        
    private:
        StmlError()
        {};
    };

}//end namespace
#endif /* error_h */
