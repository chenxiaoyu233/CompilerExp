#ifndef __GRAMMER_DEFINIRION_H__
#define __GRAMMER_DEFINIRION_H__

#include "FrontEnd.hpp"
/* simplify the writting */
#define P(s) hg.AddProduction(s)

/* use this class to generate mid level code */
struct MCodeBase {
    MCodeBase *chlid;
    list<string> code;
    void include(MCodeBase *other) { code.merge(other -> code); }
    virtual MCodeBase* generate() = 0;
};

/* used for storing the semantic action */
extern vector<MCodeBase*> semantic;

#define E(...) do {\
    struct MCodeBaseSub : public MCodeBase {\
        virtual MCodeBase* generate() {\
            __VA_ARGS__\
        }\
    };\
    semantic.push_back(new MCodeBaseSub());\
} while(0)

HumanGrammer GrammerDefinition();

#endif
