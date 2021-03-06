#include "MCode.hpp"

void FrontEndImplement::grammerDefinition() {
    //HumanGrammer hg(1, "program");
    /* hold place for Production S */
    //semantic.push_back(new MCodeBase());
    
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
            tp = {ch(0), tp[0], tp[1], tp[2]};
            if (ch(0) == "void") {
                ErrorReport(context).Report(
                    "error", "you can not use \'void\' to define a variable",
                    child[0] -> begin, child[0] -> end
                );
                ret -> errorCnt += 1;
            }
        }
        /* add the vars defined here to the current scope */
        for (auto &tp: ret -> code) if (!symbolDuplicate(tp[1])) {
            addSymbol(MCodeSymbol{tp[0], tp[1], atoi(tp[2].c_str())});
        } else {
            ErrorReport(context).Report(
                "error", "the symbol \'" + tp[1] + "\' is duplicate",
                ret -> begin, ret -> end
            );
            ret -> errorCnt += 1;
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
        ret -> include({ch(0), "0", "0"});
        ErrorReport(context).Report(
            "warning", "variable \'" + ch(0) + "\' do not initalize",
            child[0] -> begin, child[0] -> end
        );
        ret -> warnCnt += 1;
    );
    PE("var-decl-id -> ID = NUM",
        ret -> include({ch(0), "0", ch(2)});
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
        MCodeSymbol s = newVar("int", 0);
        ret -> include({"var", s.name});
        ret -> include({s.name, "=", "0"});
        ret -> include({"return", s.name});
        ret -> include({"end"});
        /* close the current scope */
        closeScope();
    );
    PE("function-head -> type-specifier ID ( params )",
        /* generate function name */
        string func_name = ch(1);
        /* check for main() */
        if (func_name == "main") {
            if (!(child[3] -> code).empty()) {
                ErrorReport(context).Report(
                    "error", "the \'main\' function should not have any parameters",
                    child[1] -> begin, child[1] -> end
                );
                ret -> errorCnt += 1;
            }
        }
        /* check if function token duplicate */
        if (symbolDuplicate(func_name)) {
            ErrorReport(context).Report(
                "error", "the function \'" + func_name + "\' is duplicate",
                child[1] -> begin, child[1] -> end
            );
            ret -> errorCnt += 1;
        }
        /* add the function to symbol table */
        addSymbol(MCodeSymbol{"label", func_name, 0});
        /* open a new scope */
        openScope();
        /* add the var in param list in current scope */
        for (auto tp: child[3] -> code) if (!symbolDuplicate(tp[1])) {
            addSymbol(MCodeSymbol{"var", tp[1], atoi(tp[2].c_str())});
        } else {
            ErrorReport(context).Report(
                "error", "the symbol \'" + tp[1] + "\' is duplicate",
                child[3] -> begin, child[3] -> end
            );
            ret -> errorCnt += 1;
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
            ErrorReport(context).Report(
                "error", "you can not use a \'void\' variable",
                child[0] -> begin, child[0] -> end
            );
            ret -> errorCnt += 1;
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
        ret -> include({"#begin"});
        ret -> include(child[1]);
        ret -> include(child[2]);
        ret -> include({"#end"});
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
    PE("statement -> expression-stmt",
        ret -> include({"#begin"});
        ret -> include(child[0]);
        ret -> include({"#end"});
    );
    PE("statement -> compound-stmt",ret -> include(child[0]););
    PE("statement -> selection-stmt",
        ret -> include({"#begin"});
        ret -> include(child[0]);
        ret -> include({"#end"});
    );
    PE("statement -> iteration-stmt",
        ret -> include({"#begin"});
        ret -> include(child[0]);
        ret -> include({"#end"});
    );
    PE("statement -> return-stmt",
        ret -> include({"#begin"});
        ret -> include(child[0]);
        ret -> include({"#end"});
    );
    PE("statement -> out-stmt", ret -> include(child[0]););
    PE("out-stmt -> out ( expression ) ;",
        ret -> include(child[2]);
        ret -> include({"out", ch(2)});
    );
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
        MCodeSymbol v1 = newVar("int", 0);
        ret -> include({"var", v1.name});
        ret -> include({"label", L1.name});
        ret -> include({"#begin"});
        ret -> include(child[2]);
        ret -> include({v1.name, "=", ch(2)});
        ret -> include({"#end"});
        ret -> include({"ifz", v1.name, "goto", L2.name});
        ret -> include(child[4]);
        ret -> include({"goto", L1.name});
        ret -> include({"label", L2.name});
    );
    PE("iteration-stmt -> for ( expression ; expression ; expression ) statement",
        MCodeSymbol L1 = newLabel();
        MCodeSymbol L2 = newLabel();
        MCodeSymbol v1 = newVar("int", 0);
        ret -> include({"#begin"});
        ret -> include(child[2]);
        ret -> include({"#end"});
        ret -> include({"var", v1.name});
        ret -> include({"label", L1.name});
        ret -> include({"#begin"});
        ret -> include(child[4]);
        ret -> include({v1.name, "=", ch(4)});
        ret -> include({"#end"});
        ret -> include({"ifz", v1.name, "goto", L2.name});
        ret -> include({"#begin"});
        ret -> include(child[8]);
        ret -> include(child[6]);
        ret -> include({"#end"});
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
        ret -> include(child[2]);
        ret -> include({*(string*)(au(0, "var")), "=", *(string*)(au(0, "var")), "+", ch(2)});
        delete (string*)(au(0, "var"));
    );
    PE("expression -> var -= expression",
        ret -> include(child[2]);
        ret -> include({*(string*)(au(0, "var")), "=", *(string*)(au(0, "var")), "-", ch(2)});
        delete (string*)(au(0, "var"));
    );
    PE("expression -> simple-expression", ret -> include(child[0]););
    PE("var -> ID",
        /* check if the symbol exists */
        if (!symbolExists(ch(0))) {
            ErrorReport(context).Report(
                "error", "the symbol \'" + ch(0) + "\' does not exist",
                ret -> begin, ret -> end
            );
            ret -> errorCnt += 1;
        }
        /* check if the usage of this var is right */
        if (ret -> errorCnt == 0 && findSymbol(ch(0)).len > 0) {
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
            ErrorReport(context).Report(
                "error", "the symbol \'" + ch(0) + "\' does not exist",
                ret -> begin, ret -> end
            );
            ret -> errorCnt += 1;
        }
        /* check if the useage of this var is right */
        if (ret -> errorCnt == 0 && findSymbol(ch(0)).len == 0) {
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
        ErrorReport(context).Report(
            "error", "sorry, I have not implement this operator yet",
            child[1] -> begin, child[1] -> end
        );
        ret -> errorCnt += 1;
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0), "|",  ch(2)});
    );
    PE("simple-expression -> or-expression", ret -> include(child[0]););
    PE("or-expression -> or-expression & unary-rel-expression",
        ErrorReport(context).Report(
            "error", "sorry, I have not implement this operator yet",
            child[1] -> begin, child[1] -> end
        );
        ret -> errorCnt += 1;
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", ch(0), "&", ch(2)});
    );
    PE("or-expression -> unary-rel-expression", ret -> include(child[0]););
    PE("unary-rel-expression -> ! unary-rel-expression",
        ErrorReport(context).Report(
            "error", "sorry, I have not implement this operator yet",
            child[0] -> begin, child[0] -> end
        );
        ret -> errorCnt += 1;
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include(child[1]);
        ret -> include({"var", tmp.name});
        ret -> include({tmp.name, "=", "!", ch(0)});
    );
    PE("unary-rel-expression -> rel-expression", ret -> include(child[0]););
    PE("rel-expression -> add-expression relop add-expression",
        MCodeSymbol tmp = newVar("int", 0);
        MCodeSymbol flag = newVar("int", 0);
        MCodeSymbol val = newVar("int", 0);
        MCodeSymbol L1 = newLabel();
        MCodeSymbol L2 = newLabel();
        ret -> include(child[0]);
        ret -> include(child[2]);
        ret -> include({"var", tmp.name});
        ret -> include({"var", flag.name});
        ret -> include({"var", val.name});
        ret -> include({val.name, "=", ch(1, 0)});
        ret -> include({tmp.name, "=", ch(0), "-", ch(2)});
        ret -> include({flag.name, "=", "TST", tmp.name});
        ret -> include({flag.name, "=", flag.name, "-", val.name});
        ret -> include({ch(1, 1), flag.name, "goto", L1.name});
        ret -> include({val.name, "=", "0"});
        ret -> include({"goto", L2.name});
        ret -> include({"label", L1.name});
        ret -> include({val.name, "=", "1"});
        ret -> include({"label", L2.name});
        ret -> include({flag.name, "=", val.name});
    );
    PE("rel-expression -> add-expression", ret -> include(child[0]););
    PE("relop -> <=", ret -> include({"2", "if"}););
    PE("relop -> <", ret -> include({"1", "ifz"}););
    PE("relop -> >", ret -> include({"2", "ifz"}););
    PE("relop -> >=", ret -> include({"1", "if"}););
    PE("relop -> ==", ret -> include({"0", "ifz"}););
    PE("relop -> !=", ret -> include({"0", "if"}););
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
        if (ch(1) == "%") {
            ErrorReport(context).Report(
                "error", "sorry, I have not implement this operator yet",
                child[1] -> begin, child[1] -> end
            );
            ret -> errorCnt += 1;
        }
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
    PE("factor -> ( expression )", ret -> include(child[1]););
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
        if (!symbolExists(ch(0))) {
            ErrorReport(context).Report(
                "error", "the function \'" + ch(0) + "\' does not exist",
                child[0] -> begin, child[0] -> end
            );
            ret -> errorCnt += 1;
        }
        ret -> include(child[2]);
        MCodeSymbol tmp = newVar("int", 0);
        ret -> include({"var", tmp.name});
        if (au(2, "actual") != NULL) {
            ret -> include((MCodeBase*)(au(2, "actual")));
            delete (MCodeBase*)(au(2, "actual"));
        }
        ret -> include({"call", tmp.name, "=", ch(0)});
        MCodeSymbol tmp1 = newVar("int", 0);
        ret -> include({"var", tmp1.name});
        ret -> include({tmp1.name, "=", tmp.name});
    );
    PE("args -> arg-list",
       ret -> include(child[0]);
       (ret -> info)["actual"] = au(0, "actual");
    );
    PE("args ->", /* do nothing here */);
    PE("arg-list -> arg-list , expression",
        ret -> include(child[0]);
        ret -> include(child[2]);
        (ret -> info)["actual"] = au(0, "actual");
        MCodeBase* cur = (MCodeBase*)(au(0, "actual"));
        cur -> include({"actual", ch(2)});
    );
    PE("arg-list -> expression",
        ret -> include(child[0]);
        //ret -> include({"actual", ch(0)});
        (ret -> info)["actual"] = new MCodeBase();
        MCodeBase* cur = (MCodeBase*)((ret -> info)["actual"]);
        cur -> include({"actual", ch(0)});
    );

}
