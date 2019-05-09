#include "GrammerDefinition.hpp"

/* used for storing the semantic action */
vector<MCodeBase*> semantic;

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

HumanGrammer GrammerDefinition() {
    HumanGrammer hg(1, "program");
    PE("program -> declaration-list", ret -> include(child[0]););
    PE("declaration-list -> declaration-list declaration",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("declaration-list -> declaration", ret -> include(child[0]););
    PE("declaration -> var-declaration", ret -> include(child[0]););
    PE("declaration -> fun-declaration", ret -> include(child[0]););
    PE("var-declaration -> type-specifier var-decl-list ;",
        ret -> include(child[1]);
        /* add the type infomation to the type-specifier */
        for (auto &tp: ret -> code) { tp.op = "declare"; tp.arg[0] = ch(0); }
        /* add the vars defined here to the current scope */
        for (auto &tp: ret -> code) if (!symbolDuplicate(tp.arg[1])) {
            addSymbol(MCodeSymbol{tp.arg[0], tp.arg[1], atoi(tp.arg[2].c_str())});
        } else {
            fprintf(stderr, "the symbol %s is duplicate\n", tp.arg[1].c_str());
            exit(233);
        }
    );
    PE("var-decl-list -> var-decl-list , var-decl-id",
        ret -> include(child[0]);
        ret -> include(child[2]);
    );
    PE("var-decl-list -> var-decl-id",
        ret -> include(child[0]);
    );
    PE("var-decl-id -> ID",
        ret -> include(MCodeTuple("", "", ch(0), "0"));
    );
    PE("var-decl-id -> ID [ NUM ]",
        ret -> include(MCodeTuple("", "", ch(0), ch(2)));
    );
    PE("type-specifier -> int", 
        ret -> include(MCodeTuple("", "int", "", ""));
    );
    PE("type-specifier -> void", 
        ret -> include(MCodeTuple("", "void", "", ""));
    );
    PE("type-specifier -> bool", 
        ret -> include(MCodeTuple("", "bool", "", ""));
    );
    PE("fun-declaration -> function-head statement",
        ret -> include(child[0]);
        ret -> include(child[1]);
        ret -> include(MCodeTuple("func-end", "", "", ""));
        /* close the current scope */
        closeScope();
        ret -> include(MCodeTuple("popto", "#", "", ""));
    );
    PE("function-head -> type-specifier ID ( params )",
        /* generate function name */
        string func_name = ch(1);
        for (auto tp: child[3] -> code) {
            func_name += "_";
            func_name += tp.arg[0];
        }
        /* check if function token duplicate */
        if (symbolDuplicate(func_name)) {
            fprintf(stderr, "the symbol %s is duplicate\n", func_name.c_str());
            exit(233);
        }
        /* add the function to symbol table */
        addSymbol(MCodeSymbol{"function", func_name, 0});
        /* open a new scope */
        openScope();
        ret -> include(MCodeTuple("push", "#", "", ""));
        /* add the var in param list in current scope */
        for (auto tp: child[3] -> code) if (!symbolDuplicate(tp.arg[1])) {
            addSymbol(MCodeSymbol{tp.arg[0], tp.arg[1], atoi(tp.arg[2].c_str())});
        } else {
            fprintf(stderr, "the sumbol %s is duplicate\n", tp.arg[1].c_str());
            exit(233);
        }
        /* generate the MCode */
        ret -> include(MCodeTuple("function", func_name, "", ""));
        ret -> include(MCodeTuple("func-begin", "", "", ""));
        ret -> include(child[3]);
    );
    PE("params -> param-list", ret -> include(child[0]););
    PE("params ->", /* do nothing here */);
    PE("param-list -> param-list , param-type-list",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("param-list -> param-type-list", ret -> include(child[0]););
    PE("param-type-list -> type-specifier param-id",
        ret -> include(MCodeTuple("formal", ch(0), ch(1, 1), ch(1, 2)));
    );
    PE("param-id -> ID", 
        ret -> include(MCodeTuple("", "", ch(0), "0"));
    );
    PE("param-id -> ID [ ]",
        ret -> include(MCodeTuple("", "", ch(0), "1"));
    );
    PE("compound-stmt -> local-scope-start local-declarations statement-list }",
        ret -> include(child[1]);
        ret -> include(child[2]);
        /* close the scope */
        closeScope();
    );
    PE("local-scope-start -> {",
        /* open a new scope */
        openScope();
    );
    PE("local-declarations -> local-declarations var-declaration",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("local-declarations ->", /* do nothing here */);
    PE("statement-list -> statement-list statement",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("statement-list ->", /* do nothing here */);
    PE("statement -> expression-stmt", ret -> include(child[0]););
    PE("statement -> compound-stmt", ret -> include(child[0]););
    PE("statement -> selection-stmt", ret -> include(child[0]););
    PE("statement -> iteration-stmt", ret -> include(child[0]););
    PE("statement -> return-stmt", ret -> include(child[0]););
    PE("statement -> break-stmt", ret -> include(child[0]););
    PE("expression-stmt -> expression ;", ret -> include(child[0]););
    PE("expression-stmt -> ;", /* do nothing here */);
    PE("selection-stmt -> if ( expression ) statement",
        MCodeSymbol L = newLabel();
        ret -> include(child[2]);
        ret -> include(MCodeTuple("ifz", (child[2] -> code).back().arg[0], "goto", L.name));
        ret -> include(child[4]);
        ret -> include(MCodeTuple("label", L.name, "", ""));
    );
    PE("selection-stmt -> if ( expression ) statement else statement",
        MCodeSymbol L1 = newLabel();
        MCodeSymbol L2 = newLabel();
        ret -> include(child[2]);
        ret -> include(MCodeTuple("ifz", (child[2] -> code).back().arg[0], "goto", L1.name));
        ret -> include(child[4]);
        ret -> include(MCodeTuple("goto", L2.name, "", ""));
        ret -> include(MCodeTuple("label", L1.name, "", ""));
        ret -> include(child[6]);
        ret -> include(MCodeTuple("label", L2.name, "", ""));
    );
    P("iteration-stmt -> while ( expression ) statement");
    P("return-stmt -> return ; | return expression ;");
    P("break-stmt -> break ;");
    PE("expression -> var = expression",
        ret -> include(child[2]);
        ret -> include(MCodeTuple("=", ch(0), (child[2] -> code).back().arg[0], ""));
    );
    PE("expression -> var += expression",
        ret -> include(child[2]);
        ret -> include(MCodeTuple("+", ch(0), ch(0), (child[2] -> code).back().arg[0]));
    );
    PE("expression -> var −= expression",
        ret -> include(child[2]);
        ret -> include(MCodeTuple("-", ch(0), ch(0), (child[2] -> code).back().arg[0]));
    );
    PE("expression -> simple-expression", ret -> include(child[0]););
    PE("var -> ID",
        /* check if the symbol exists */
        if (!symbolExists(ch(0))) {
            fprintf(stderr, "the symbol %s does not exist\n", ch(0).c_str());
            exit(233);
        }
        /* check if the usage of this var is right */
        if (findSymbol(ch(0)).len > 0) {
            fprintf(stderr, "the symbol %s is an array, it should be followed by []\n", ch(0).c_str());
            exit(233);
        }
        /* genreate the code */
        ret -> include(MCodeTuple("", "", ch(0), 0));
    );
    PE("var -> ID [ expression ]",
        /* check if the symbol exists */
        if (!symbolExists(ch(0))) {
            fprintf(stderr, "the symbol %s does not exist\n", ch(0).c_str());
            exit(233);
        }
        /* check if the useage of this var is right */
        if (findSymbol(ch(0)).len == 0) {
            fprintf(stderr, "the symbol %s is not an array, it should not be followed by []\n", ch(0).c_str());
            exit(233);
        }
        /* generate the code */
        ret -> include(child[2]);
        ret -> include(MCodeTuple("", ch(0) + "[" + (child[2] -> code).back().arg[0] + "]", "", ""));
    );
    PE("simple-expression -> simple-expression \\| or-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include(MCodeTuple("declare", "int", tmp.name, "0"));
        ret -> include(MCodeTuple("|", tmp.name, (child[0] -> code).back().arg[0], (child[2] -> code).back().arg[0]));
    );
    PE("simple-expression -> or-expression", ret -> include(child[0]););
    PE("or-expression -> or-expression & unary-rel-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include(MCodeTuple("declare", "int", tmp.name, "0"));
        ret -> include(MCodeTuple("&", tmp.name, (child[0] -> code).back().arg[0], (child[2] -> code).back().arg[0]));
    );
    PE("or-expression -> unary-rel-expression", ret -> include(child[0]););
    PE("unary-rel-expression -> ! unary-rel-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[1]);
        ret -> include(MCodeTuple("declare", "int", tmp.name, "0"));
        ret -> include(MCodeTuple("!", tmp.name, (child[1] -> code).back().arg[0], ""));
    );
    PE("unary-rel-expression -> rel-expression", ret -> include(child[0]););
    PE("rel-expression -> add-expression relop add-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include(MCodeTuple("declare", "int", tmp.name, "0"));
        ret -> include(MCodeTuple(ch(1), tmp.name, (child[0] -> code).back().arg[0], (child[2] -> code).back().arg[0]));
    );
    PE("rel-expression -> add-expression", ret -> include(child[0]););
    PE("relop -> <=", ret -> include(MCodeTuple("", "<=", "", "")););
    PE("relop -> <", ret -> include(MCodeTuple("", "<", "", "")););
    PE("relop -> >", ret -> include(MCodeTuple("", ">", "", "")););
    PE("relop -> >=", ret -> include(MCodeTuple("", ">=", "", "")););
    PE("relop -> ==", ret -> include(MCodeTuple("", "==", "", "")););
    PE("relop -> !=", ret -> include(MCodeTuple("", "!=", "", "")););
    P("add-expression -> add-expression addop term | term");
    PE("addop -> +", ret -> include(MCodeTuple("", "+", "", "")););
    PE("addop -> -", ret -> include(MCodeTuple("", "-", "", "")););
    P("term -> term mulop unary-expression | unary-expression");
    PE("mulop -> *", ret -> include(MCodeTuple("", "*", "", "")););
    PE("mulop -> /", ret -> include(MCodeTuple("", "/", "", "")););
    PE("mulop -> %", ret -> include(MCodeTuple("", "%", "", "")););
    P("unary-expression -> - unary-expression | factor");
    P("factor -> ( expression ) | var | call | constant");
    P("constant -> NUM | true | false");
    P("call -> ID ( args )");
    P("args -> arg-list |");
    P("arg-list -> arg-list , expression | expression");

    hg.BuildIT();
    hg.BuildcE();
    return hg;
}
