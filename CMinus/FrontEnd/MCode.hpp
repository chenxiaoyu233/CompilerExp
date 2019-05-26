#ifndef __M_CODE_H__
#define __M_CODE_H__

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

#endif
