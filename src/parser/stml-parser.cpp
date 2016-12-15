//
//  stml-parser.cpp
//  Smobiler
//
//  Created by jinyk on 16/5/18.
//
//

#include <stdio.h>
#include "stml-parser.h"


const char *CREATE = "CREATE";

namespace STMLParser
{
    Parser& Parser::instance()
    {
        static Parser s_parser;
        return s_parser;
    }
    
    Parser::Parser()
    {
    }
    
    void Parser::parse()
    {
        if(m_pause) return;
        if(m_dataQueue.empty()) return;
        auto& pDocument = m_dataQueue.front();
        XMLElement* root = pDocument->RootElement();
        if(!root) return;
        if(!m_checked)
        {
            XMLElement *entity = root->FirstChildElement();
            if (entity) {
                const char *name = entity->Name();
                if (strcmp(name, CREATE) == 0)
                {
                    const char *type = entity->Attribute("Type");
                    if (strcmp(type, "Form") == 0) {
                        pause();
                        m_checked = true;
                        if(m_pause) return;
                    }
                }
            }
        }

        if (m_processor) m_processor(move(pDocument));
        m_dataQueue.pop();
        m_checked = false;
    }

}//end namespace STMLParser
