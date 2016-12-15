
#ifndef __stml_parser__
#define __stml_parser__

#include <iostream>
#include <queue>
#include <memory>
#include <initializer_list>

#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

namespace STMLParser
{
    
    class Parser 
    {
    public:
        
        typedef unique_ptr<tinyxml2::XMLDocument> DataType;
    
    public:
        static Parser& instance();
        
        Parser();
        
        void parse();
        void pause()
        {
            m_pause = true;
        }
        void resume()
        {
            m_pause = false;
        }
        void enqueue(DataType&& data)
        {
            m_dataQueue.emplace(move(data));
        }
        void setProcessor(const function<void(const DataType)>& func)
        {
            m_processor = func;
        }
        
    private:
        queue<DataType> m_dataQueue;
        function<void(const DataType)> m_processor;
        bool m_pause = false;
        bool m_checked = false;
    };

}
#endif
