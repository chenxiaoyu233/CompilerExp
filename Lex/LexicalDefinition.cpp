LexicalAnalyzer* LexicalDefinition() {
	size_t keywordLevel = 10, constantLevel = 2, symbolLevel = 1;
	LexicalAnalyzer *lex = new LexicalAnalyzer();
	// white space
	lex -> AddLexicalItem("\\[ \t\n\\]\\{1, \\}", "WHITE-SPACE", false);

	// symbol
	lex -> AddLexicalItem("\
		\\[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\\] \\+ \
		\\(\
			\\[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\\] \\|\
			\\[0123456789\\] \\|\
			\\[_\\]\
		\\)\\{, \\}", "SYMBOL", true, symbolLevel);

	// constant
	lex -> AddLexicalItem("\\[123456789\\] \\+ \\[0123456789\\]\\{, \\}", "INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\\[123456789\\] \\+ \\[0123456789\\]\\{, \\} \\+ \\[Ll\\]", "LONG-INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\\$0\\$ \\+ \\[xX\\] \\+ \\[0123456789abcdeABCDE\\]", "HEX-INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\\$0\\$ \\+ \\[xX\\] \\+ \\[0123456789abcdeABCDE\\] \\+ \\[Ll\\]", "HEX-LONG-INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\
		\\(\
			\\[123456789\\] \\+ \\[0123456789\\]\\{, \\} \\|\
			\\$0\\$\
		\\)\\{0, 1\\} \\+ \\$.\\$ \\+ \\[0123456789\\]\\{1, \\} \\+ \
		\\(																\
			\\$e\\$ \\+ \\[-\\]\\{0, 1\\} \\+ \\[123456789\\] \\+ \\[0123456789\\]\\{, \\}\
		\\)\\{0, 1\\}", "FLOAT-CONSTANT", true, constantLevel);

	// type ,class, declare
	lex -> AddLexicalItem("\\$int\\$", "INT-TYPE", true, keywordLevel);
	lex -> AddLexicalItem("\\$float\\$", "FLOAT-TYPE", true, keywordLevel);
	lex -> AddLexicalItem("\\$double\\$", "DOUBLE-TYPE", true, keywordLevel);
	lex -> AddLexicalItem("\\$char\\$", "CHAR-TYPE", true, keywordLevel);
	lex -> AddLexicalItem("\\$class\\$", "CLASS", true, keywordLevel);
	lex -> AddLexicalItem("\\$struct\\$", "STRUCT", true, keywordLevel);
	lex -> AddLexicalItem("\\$const\\$", "CONST", true, keywordLevel);
	lex -> AddLexicalItem("\\$static\\$", "STATIC", true, keywordLevel);
	lex -> AddLexicalItem("\\$extern\\$", "EXTERN", true, keywordLevel);

	// control
	lex -> AddLexicalItem("\\$if\\$", "IF", true, keywordLevel);
	lex -> AddLexicalItem("\\$else\\$", "ELSE", true, keywordLevel);
	lex -> AddLexicalItem("\\$while\\$", "WHILE", true, keywordLevel);
	lex -> AddLexicalItem("\\$for\\$", "FOR", true, keywordLevel);
	lex -> AddLexicalItem("\\$do\\$", "DO", true, keywordLevel);
	lex -> AddLexicalItem("\\$goto\\$", "GOTO", true, keywordLevel);

	// operator and expression
	lex -> AddLexicalItem("\\$+\\$", "+", true);
	lex -> AddLexicalItem("\\$-\\$", "-", true);
	lex -> AddLexicalItem("\\$*\\$", "*", true);
	lex -> AddLexicalItem("\\$/\\$", "/", true);
	lex -> AddLexicalItem("\\$<\\$", "<", true);
	lex -> AddLexicalItem("\\$<=\\$", "<=", true);
	lex -> AddLexicalItem("\\$>=\\$", ">=", true);
	lex -> AddLexicalItem("\\$>\\$", ">", true);
	lex -> AddLexicalItem("\\$!=\\$", "!=", true);
	lex -> AddLexicalItem("\\$==\\$", "==", true);
	lex -> AddLexicalItem("\\$=\\$", "=", true);
	lex -> AddLexicalItem("\\$+=\\$", "+=", true);
	lex -> AddLexicalItem("\\$-=\\$", "-=", true);
	lex -> AddLexicalItem("\\$*=\\$", "*=", true);
	lex -> AddLexicalItem("\\$/=\\$", "/=", true);
	lex -> AddLexicalItem("\\$^=\\$", "^=", true);
	lex -> AddLexicalItem("\\$&=\\$", "&=", true);
	lex -> AddLexicalItem("\\$<<\\$", "<<", true);
	lex -> AddLexicalItem("\\$>>\\$", ">>", true);
	lex -> AddLexicalItem("\\$&\\$", "&", true);
	lex -> AddLexicalItem("\\$|\\$", "|", true);
	lex -> AddLexicalItem("\\$^\\$", "^", true);
	lex -> AddLexicalItem("\\$?\\$", "?", true);
	lex -> AddLexicalItem("\\$(\\$", "(", true);
	lex -> AddLexicalItem("\\$)\\$", ")", true);
	lex -> AddLexicalItem("\\$[\\$", "[", true);
	lex -> AddLexicalItem("\\$]\\$", "]", true);
	lex -> AddLexicalItem("\\${\\$", "{", true);
	lex -> AddLexicalItem("\\$}\\$", "}", true);
	lex -> AddLexicalItem("\\$,\\$", ",", true);
	lex -> AddLexicalItem("\\$;\\$", ";", true);
	lex -> AddLexicalItem("\\$:\\$", ":", true);
	lex -> AddLexicalItem("\\$::\\$", "::", true);

	return lex;
}
