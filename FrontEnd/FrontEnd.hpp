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
    /* Add a series of productions from a string (syntax in books)*/
    /* e.i. "A -> AB | B | e" (here the 'e' means empty string) */
    void AddProduction(string s);
    void BuildcE();
};

#endif
