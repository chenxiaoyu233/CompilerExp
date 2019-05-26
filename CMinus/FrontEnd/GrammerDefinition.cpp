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
    /* hold place for Production S */
    semantic.push_back(new MCodeBase());
    
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
        for (auto &tp: ret -> code) { 
            tp = {ch(0), tp[0], tp[1]}; 
            if (ch(0) == "void") {
                fprintf(stderr, "you can not use a void to define a variable\n");
                exit(233);
            }
        }
        /* add the vars defined here to the current scope */
        for (auto &tp: ret -> code) if (!symbolDuplicate(tp[1])) {
            addSymbol(MCodeSymbol{tp[0], tp[1], atoi(tp[2].c_str())});
        } else {
            /* debug */
            for (auto s: tp) {
                fprintf(stderr, "%s ", s.c_str());
            } fprintf(stderr, "\n");
            fprintf(stderr, "the symbol %s is duplicate\n", tp[1].c_str());
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
        ret -> include({ch(0), "0"});
    );
    PE("var-decl-id -> ID [ NUM ]",
        ret -> include({ch(0), ch(2)});
    );
    PE("type-specifier -> int", 
        ret -> include({"var"});
    );
    PE("type-specifier -> void", 
        ret -> include({"void"});
    );
    PE("type-specifier -> bool", 
        ret -> include({"var"});
    );
    PE("fun-declaration -> function-head statement",
        ret -> include(child[0]);
        ret -> include(child[1]);
        ret -> include({"end"});
        /* close the current scope */
        closeScope();
    );
    PE("function-head -> type-specifier ID ( params )",
        /* generate function name */
        string func_name = ch(1);

        /* check if function token duplicate */
        if (symbolDuplicate(func_name)) {
            fprintf(stderr, "the function %s is duplicate\n", func_name.c_str());
            exit(233);
        }
        /* add the function to symbol table */
        addSymbol(MCodeSymbol{"label", func_name, 0});
        /* open a new scope */
        openScope();
        /* add the var in param list in current scope */
        for (auto tp: child[3] -> code) if (!symbolDuplicate(tp[1])) {
            addSymbol(MCodeSymbol{"var", tp[1], atoi(tp[2].c_str())});
        } else {
            fprintf(stderr, "the sumbol %s is duplicate\n", tp[1].c_str());
            exit(233);
        }
        /* generate the MCode */
        ret -> include({"label", func_name});
        ret -> include({"begin"});
        for (auto pt: child[3] -> code) {
            if (pt[2] == "0") ret -> include({pt[0], pt[1]});
            else ret -> include(pt);
        }
    );
    PE("params -> param-list", ret -> include(child[0]););
    PE("params ->", /* do nothing here */);
    PE("param-list -> param-list , param-type-list",
        ret -> include(child[0]);
        ret -> include(child[2]);
    );
    PE("param-list -> param-type-list", ret -> include(child[0]););
    PE("param-type-list -> type-specifier param-id",
        if (ch(0) == "void") {
            fprintf(stderr, "you can not use a void variable\n");
            exit(233);
        }
        ret -> include({"formal", ch(1, 0), ch(1, 1)});
    );
    PE("param-id -> ID",
        ret -> include({ch(0), "0"});
    );
    PE("param-id -> ID [ ]",
        ret -> include({ch(0), "1"});
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
    //PE("statement -> break-stmt", ret -> include(child[0]););
    PE("expression-stmt -> expression ;", ret -> include(child[0]););
    PE("expression-stmt -> ;", /* do nothing here */);
    PE("selection-stmt -> if ( expression ) statement",
        MCodeSymbol L = newLabel();
        ret -> include(child[2]);
        ret -> include({"ifz", ch(2), "goto", L.name});
        ret -> include(child[4]);
        ret -> include({"label", L.name});
    );
    PE("selection-stmt -> if ( expression ) statement else statement",
        MCodeSymbol L1 = newLabel();
        MCodeSymbol L2 = newLabel();
        ret -> include(child[2]);
        ret -> include({"ifz", ch(2), "goto", L1.name});
        ret -> include(child[4]);
        ret -> include({"goto", L2.name});
        ret -> include({"label", L1.name});
        ret -> include(child[6]);
        ret -> include({"label", L2.name});
    );
    PE("iteration-stmt -> while ( expression ) statement",
        MCodeSymbol L1 = newLabel();
        MCodeSymbol L2 = newLabel();
        ret -> include({"label", L1.name});
        ret -> include(child[2]);
        ret -> include({"ifz", ch(2), "goto", L2.name});
        ret -> include(child[4]);
        ret -> include({"goto", L1.name});
        ret -> include({"label", L2.name});
    );
    PE("return-stmt -> return ;",
        ret -> include({"return"});
    );
    PE("return-stmt -> return expression ;",
        ret -> include(child[1]);
        ret -> include({"return", ch(1)});
    );
    //P("break-stmt -> break ;");
    PE("expression -> var = expression",
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({*(string*)(au(0, "var")), "=", ch(2)});
        delete (string*)(au(0, "var"));
    );
    PE("expression -> var += expression",
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({*(string*)(au(0, "var")), "=", ch(0), "+", ch(2)});
        delete (string*)(au(0, "var"));
    );
    PE("expression -> var −= expression",
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({*(string*)(au(0, "var")), "=", ch(0), "-", ch(2)});
        delete (string*)(au(0, "var"));
    );
    PE("expression -> simple-expression", ret -> include(child[0]););
    PE("var -> ID",
        /* check if the symbol exists */
        if (!symbolExists(ch(0))) {
            fprintf(stderr, "var -> ID: the symbol %s does not exist\n", ch(0).c_str());
            exit(233);
        }
        /* check if the usage of this var is right */
        if (findSymbol(ch(0)).len > 0) {
            fprintf(stderr, "the symbol %s is an array, it should be followed by []\n", ch(0).c_str());
            exit(233);
        }
        /* genreate the code */
        string *s = new string();
        *s = ch(0);
        (ret -> info)["var"] = s;
    );
    PE("var -> ID [ expression ]",
        /* check if the symbol exists */
        if (!symbolExists(ch(0))) {
            fprintf(stderr, "var -> ID [ expression ] : the symbol %s does not exist\n", ch(0).c_str());
            exit(233);
        }
        /* check if the useage of this var is right */
        if (findSymbol(ch(0)).len == 0) {
            fprintf(stderr, "the symbol %s is not an array, it should not be followed by []\n", ch(0).c_str());
            exit(233);
        }
        /* generate the code */
        ret -> include(child[2]);
        string *s = new string();
        *s = ch(0) + "[" + ch(2) + "]";
        (ret -> info)["var"] = s;
    );
    PE("simple-expression -> simple-expression \\| or-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0), "|",  ch(2)});
    );
    PE("simple-expression -> or-expression", ret -> include(child[0]););
    PE("or-expression -> or-expression & unary-rel-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0), "&", ch(2)});
    );
    PE("or-expression -> unary-rel-expression", ret -> include(child[0]););
    PE("unary-rel-expression -> ! unary-rel-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[1]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", "!", ch(0)});
    );
    PE("unary-rel-expression -> rel-expression", ret -> include(child[0]););
    PE("rel-expression -> add-expression relop add-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0), ch(1), ch(2)});
    );
    PE("rel-expression -> add-expression", ret -> include(child[0]););
    PE("relop -> <=", ret -> include({"<="}););
    PE("relop -> <", ret -> include({"<"}););
    PE("relop -> >", ret -> include({">"}););
    PE("relop -> >=", ret -> include({">="}););
    PE("relop -> ==", ret -> include({"=="}););
    PE("relop -> !=", ret -> include({"!="}););
    PE("add-expression -> add-expression addop term",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0), ch(1), ch(2)});
    );
    PE("add-expression -> term", ret -> include(child[0]););
    PE("addop -> +", ret -> include({"+"}););
    PE("addop -> -", ret -> include({"-"}););
    PE("term -> term mulop unary-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0), ch(1), ch(2)});
    );
    PE("term -> unary-expression", ret -> include(child[0]););
    PE("mulop -> *", ret -> include({"*"}););
    PE("mulop -> /", ret -> include({"/"}););
    PE("mulop -> %", ret -> include({"%"}););
    PE("unary-expression -> - unary-expression",
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[1]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", "-", ch(1)});
    );
    PE("unary-expression -> factor", ret -> include(child[0]););
    PE("factor -> ( expression )", ret -> include(child[0]););
    PE("factor -> var",
        ret -> include(child[0]);
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", *(string*)(au(0, "var"))});
    );
    PE("factor -> call", ret -> include(child[0]););
    PE("factor -> constant", ret -> include(child[0]););
    PE("constant -> NUM", 
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0)});
    );
    PE("constant -> true", 
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", "1"});
    );
    PE("constant -> false", 
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", "0"});
    );
    PE("call -> ID ( args )",
        ret -> include(child[2]);
        ret -> include((MCodeBase*)(au(2, "actual")));
        delete (MCodeBase*)(au(2, "actual"));
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", "call", ch(0)});
    );
    PE("args -> arg-list", ret -> include(child[0]););
    PE("args ->", /* do nothing here */);
    PE("arg-list -> arg-list , expression",
        ret -> include(child[2]);
        (ret -> info)["actual"] = au(0, "actual");
        MCodeBase* cur = (MCodeBase*)(au(0, "actual"));
        cur -> include({"actual", ch(0)});
    );
    PE("arg-list -> expression",
        ret -> include(child[0]);
        //ret -> include({"actual", ch(0)});
        (ret -> info)["actual"] = new MCodeBase();
        MCodeBase* cur = (MCodeBase*)((ret -> info)["actual"]);
        cur -> include({"actual", ch(0)});
    );

    hg.BuildIT();
    hg.BuildcE();
    return hg;
}
