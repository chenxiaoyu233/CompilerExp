/* simplify the writting */
#define P(s) hg.AddProduction(s)

HumanGrammer GrammerDefinition() {
    HumanGrammer hg;
    P("program -> declaration-list");
    P("declaration-list -> declaration-list declaration | declaration");
    P("declaration -> var-declaration | fun-declaration");
    P("var-declaration -> type-specifier var-decl-list ;");
    P("var-decl-list -> var-decl-list , var-decl-id | var-decl-id");
    P("var-decl-id -> ID | ID [ NUM ]");
    P("type-specifier -> int | void | bool");
    P("fun-declaration -> type-specifier ID ( params ) statement");
    P("params -> param-list | ");
    P("param-list -> param-list ; param-type-list | param-type-list");
    P("param-type-list -> type-specifier param-id-list");
    P("param-id-list -> param-id-list , param-id | param-id");
    P("param-id -> ID | ID [ ]");
    P("compound-stmt -> { local-declarations statement-list }");
    P("local-declarations -> local-declarations var-declaration | ");
    P("statement-list -> statement-list statement | ");
    P("statement -> expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt | break-stmt");
    P("expression-stmt -> expression ; | ;");
    P("selection-stmt -> if ( expression ) statement | if ( expression ) statement else statement");
    P("iteration-stmt -> while ( expression ) statement");
    P("return-stmt -> return ; | return expression ;");
    P("break-stmt -> break ;");
    P("expression -> var = expression | var += expression | var −= expression | simple-expression");
    P("var -> ID | ID [ expression ]");
    P("simple-expression -> simple-expression \\| or-expression | or-expression");
    P("or-expression -> or-expression & unary-rel-expression | unary-rel-expression");
    P("unary-rel-expression -> ! unary-rel-expression | rel-expression");
    P("rel-expression -> add-expression relop add-expression | add-expression");
    P("relop -> <= | < | > | >= | == | !=");
    P("add-expression -> add-expression addop term | term");
    P("addop -> + | −");
    P("term -> term mulop unary-expression | unary-expression");
    P("mulop -> ∗ | / | %");
    P("unary-expression -> - unary-expression | factor");
    P("factor -> ( expression ) | var | call | constant");
    P("constant -> NUM | true | false");
    P("call -> ID ( args )");
    P("args -> arg-list | ");
    P("arg-list -> arg-list , expression | expression");
}
