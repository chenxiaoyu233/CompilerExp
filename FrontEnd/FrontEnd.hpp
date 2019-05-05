/* This header offers some basic tools for creating a Front End */
#ifndef __FRONT_END_H__
#define __FRONT_END_H__

#include "common.hpp"
/* Lex */
#include "LexicalAnalyzer.hpp"
/* Parse */
#include "LR.hpp"


/* This is used for representing productions in a human readable way */
struct HumanProduction {
    string lhs;
    vector<string> rhs;
};

/* This is used for representing grammers in a human readable way */
struct HumanGrammer {
    vector<HumanProduction> P;
    set<string> I, T, cE;
    /* create a grammer for LR(k) parser */
    HumanGrammer();
    HumanGrammer(int k, string start);
    /* Add a series of productions from a string (syntax in books)*/
    /* e.i. "A -> AB | B | e" (here the 'e' means empty string) */
    void AddProduction(string s);
    void BuildcE();
};


/* Convert a HumanGrammer hg to a Grammer which is recognized by the parser */
LR::Grammer HG2G(HumanGrammer hg, map<string, int> &s2i, map<int, string> &i2s); 

#endif
