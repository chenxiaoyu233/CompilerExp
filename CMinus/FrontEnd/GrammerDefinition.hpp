#ifndef __GRAMMER_DEFINIRION_H__
#define __GRAMMER_DEFINIRION_H__

#include "FrontEnd.hpp"

/* use this to store a Symbol in MCode */
struct MCodeSymbol { string type; string name; int len; };

/* global symbol table */
extern list<MCodeSymbol> GST;
bool globalSymbolDuplicate(string s);
void addGlobalSymbal(MCodeSymbol sym);

/* local symbol table */
extern list<list<MCodeSymbol> > LST;
bool localSymbolDuplicate(string s);
void addLocalSymbol(MCodeSymbol sym);

/* symbol table aux function */
bool symbolDuplicate(string s);
void addSymbol(MCodeSymbol sym);
void openScope();
void closeScope();
bool symbolExists(string s);
MCodeSymbol findSymbol(string s);
/* generate temperate symbol */
extern int label_counter;
MCodeSymbol newLabel();
extern int var_counter;
MCodeSymbol newVar(string type, int len);


/* use this to store a single MCode sentence */
typedef vector<string> MCodeTuple;

/* use this class to generate mid level code */
struct MCodeBase {
    int begin, end; // [, )
    vector<MCodeBase*> child;
    list<MCodeTuple> code;
    map<string, void*> info; // aux information;
    MCodeBase() { code.clear(); child.clear(); begin = end = -1; info.clear(); }
    void include(MCodeBase *other) { for (auto item: other -> code) code.push_back(item); }
    void include(MCodeTuple ln) { code.push_back(ln); }
    string ch(int i, int j = 0) { return (child[i] -> code).back()[j]; } // child code
    void* au(int i, string label) { return (child[i] -> info)[label]; } // aux info
    virtual void generate(MCodeBase* ret) { }
};

/* used for storing the semantic action */
extern vector<MCodeBase*> semantic;

/* use this to do P and E */
#define PE(s, ...) do {\
    P(s);\
    E(__VA_ARGS__);\
} while(0)

/* use this to define a production */
#define P(s) hg.AddProduction(s)

/* use this to bind a semantic action to a production */
#define E(...) do {\
    struct MCodeBaseSub : public MCodeBase {\
        virtual void generate(MCodeBase *ret) {\
            __VA_ARGS__\
        }\
    };\
    semantic.push_back(new MCodeBaseSub());\
} while(0)

HumanGrammer GrammerDefinition();

#endif
