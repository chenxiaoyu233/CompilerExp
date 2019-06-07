#ifndef __BACK_END_IMPLEMENT_H__
#define __BACK_END_IMPLEMENT_H__

#include "FrontEnd.hpp"

namespace BackEnd{
    class BackEndImplement: public FrontEnd {
        virtual void lexDefinition();
        virtual void lexErrorHandler(LexicalAnalyzer::LexicalErrorInfo errInfo);
        virtual void grammerDefinition();
        virtual bool AfterLex();

    public:
        BackEndImplement(string context, string node_dump = "", string trans_dump = "")
        :FrontEnd(context, node_dump, trans_dump) {}
    };
}
#endif
