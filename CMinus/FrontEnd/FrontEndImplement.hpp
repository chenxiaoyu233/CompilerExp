#ifndef __FRONT_END_IMPLEMENT_H__
#define __FRONT_END_IMPLEMENT_H__

#include "FrontEnd.hpp"

class FrontEndImplement: public FrontEnd {
    virtual void lexDefinition();
    virtual void lexErrorHandler(LexicalAnalyzer::LexicalErrorInfo errInfo);
    virtual void grammerDefinition();
    virtual bool AfterLex();
    
public:
     FrontEndImplement(string context, string node_dump = "", string trans_dump = "")
        :FrontEnd(context, node_dump, trans_dump) {}
};

#endif
