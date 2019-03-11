DIGIT [0-9] 
ALPHA [a-zA-Z] 
ALNUM [0-9a-zA-Z] 
IDENT [0-9a-zA-Z_]

%%
" "|"\t"|"\n" { }

({ALPHA}|_){IDENT}* {
	printf("(%s, %s)\n", "SYMBOL",  yytext);
}

[1-9]{DIGIT}*|0 {
	printf("(%s, %s)\n", "INT-CONSTANT",  yytext);
}

([1-9]{DIGIT}*|0)[lL] {
	printf("(%s, %s)\n", "LONG-INT-CONSTANT",  yytext);
}

([1-9]{DIGIT}*|0)?\.{DIGIT}*(e(-?)[1-9]{DIGIT}*)? {
	printf("(%s, %s)\n", "FLOAT-CONSTANT",  yytext);
}

"if"|"while"|"do"|"for"|"else"|"case"|"switch"|"goto"|"return"|"static"|"const"|"using"|"namespace" {
	printf("(%s, %s)\n", "KEYWORD",  yytext);
}

"+"|"-"|"*"|"/"|"^"|"%"|"&&"|"||"|"<<"|">>"|"="|"=="|"!="|"<="|">="|"<"|">"|"."|"->"|"+="|"-="|"*="|"^="|"%="|"/="|"&=" {
	printf("(%s, %s)\n", "OPERATOR",  yytext);
}

\(|\)|\{|\}|\[|\]|; {
	printf("(%s, %s)\n", yytext,  yytext);
}
%%
int main() {
	yyin = stdin;
	yylex();
	return 0;
}
