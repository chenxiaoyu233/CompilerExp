LexicalAnalyzer* LexicalDefinition() {
	LexicalAnalyzer *lex = new LexicalAnalyzer();
	lex -> AddLexicalItem("\\[ \t\n\\]\\{, \\}", "WHITE-SPACE", false);
	lex -> AddLexicalItem("
		\\[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\\] \\+ 
		\\(
			\\[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\\] \\|
			\\[0123456789\\] \\|
			\\[_\\]
		\\)\\{, \\}", "SYMBOL", true);
	lex -> AddLexicalItem("\\[123456789\\] \\+ \\[0123456789\\]\\{, \\}", "INT-CONSTANT", true);
	lex -> AddLexicalItem("
		\\(
			\\[123456789\\] \\+ \\[0123456789\\]\\{, \\} \\|
			\\$0\\$
		\)\\{0, 1\\} \\+ \\$.\\$ \\+ \\[0123456789\\]\\{, \\}", "FLOAT-CONSTANT", true);
	lex -> AddLexicalItem("\\$int\\$", "INT-TYPE", true);
	lex -> AddLexicalItem("\\$float\\$", "FLOAT-TYPE", true);
	lex -> AddLexicalItem("\\$if\\$", "IF", true);
	lex -> AddLexicalItem("\\$else\\$", "ELSE", true);
	lex -> AddLexicalItem("\\$while\\$", "WHILE", true);
	lex -> AddLexicalItem("\\$for\\$", "FOR", true);
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
	lex -> AddLexicalItem("\\$(\\$", "(", true);
	lex -> AddLexicalItem("\\$)\\$", ")", true);
	lex -> AddLexicalItem("\\${\\$", "{", true);
	lex -> AddLexicalItem("\\$}\\$", "}", true);
	lex -> AddLexicalItem("\\$,\\$", ",", true);
	lex -> AddLexicalItem("\\$;\\$", ";", true);

	return lex;
}
