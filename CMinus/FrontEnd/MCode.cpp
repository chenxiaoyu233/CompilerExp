#include "MCode.hpp"

/* global symbol table */
list<MCodeSymbol> GST;
bool globalSymbolDuplicate(string s) {
    for (auto &sym: GST) if (sym.name == s) return true;
    return false;
}
void addGlobalSymbal(MCodeSymbol sym) { GST.push_back(sym); }

/* local symbol table */
list<list<MCodeSymbol> > LST;
bool localSymbolDuplicate(string s) {
    for (auto &sym: LST.back()) if (sym.name == s) return true;
    return false;
}
void addLocalSymbol(MCodeSymbol sym) { LST.back().push_back(sym); }

/* symbol table aux function */
bool symbolDuplicate(string s) {
    if (LST.empty()) return globalSymbolDuplicate(s);
    else return localSymbolDuplicate(s);
}
void addSymbol(MCodeSymbol sym) {
    if (LST.empty()) addGlobalSymbal(sym);
    else addLocalSymbol(sym);
}
void openScope() {
    list<MCodeSymbol> scope; scope.clear();
    LST.push_back(scope);
}
void closeScope() { LST.pop_back(); }

bool symbolExists(string s) {
    for (auto &lst: LST)
        for (auto &sym: lst) if (sym.name == s) return true;
    for (auto &sym: GST) if (sym.name == s) return true;
    return false;
}

MCodeSymbol findSymbol(string s){
    for (auto &lst: LST)
        for (auto &sym: lst) if (sym.name == s) return sym;
    for (auto &sym: GST) if (sym.name == s) return sym;
}

int label_counter = 0;
MCodeSymbol newLabel() {
    string name = "<L" + to_string(++label_counter) + ">";
    MCodeSymbol sym{"label", name, 0};
    addSymbol(sym);
    return sym;
}

int var_counter = 0;
MCodeSymbol newVar(string type, int len) {
    string name = "<T" + to_string(++var_counter) + ">";
    MCodeSymbol sym{type, name, len};
    addSymbol(sym);
    return sym;
}

