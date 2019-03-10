LexicalAnalyzer* LexicalDefinition() {
	// setting the aux charactor sets
	string az = "abcdefghijklmnopqrstuvwxyz";
	string AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string digit = "0123456789";
	string hex = "0123456789abcdeABCDE";
	string alphabet = "";
	// reverse "for" to eliminate the exists "\]" in the alphabet
	for (int i = 127; i > 0; i--) if(char(i) != '\"' && char(i) != '\'') alphabet += char(i);
	string commentCharSet = "";
	for (int i = 127; i >= -128; i--) if(char(i) != '\n') commentCharSet += char(i);
	
	size_t keywordLevel = 10, constantLevel = 2, symbolLevel = 1;
	LexicalAnalyzer *lex = new LexicalAnalyzer();
	// white space
	lex -> AddLexicalItem("\\[ \t\n\\]\\{1, \\}", "WHITE-SPACE", false);

	// comment
	lex -> AddLexicalItem("\\$//\\$ \\+ \\["+commentCharSet+"\\]\\{, \\}", "LINE-COMMENT", false);
	lex -> AddLexicalItem("\\$/*\\$", "BLOCK-COMMENT-BEGIN", false);
	lex -> AddLexicalItem("\\$*/\\$", "BLOCK-COMMENT-END", false);

	// symbol
	lex -> AddLexicalItem("\
		\\["+az+AZ+"_\\] \\+ \
		\\(\
			\\["+az+AZ+"\\] \\|\
			\\["+digit+"\\] \\|\
			\\[_\\]\
		\\)\\{, \\}", "SYMBOL", true, symbolLevel);

	// constant
	lex -> AddLexicalItem("\\[123456789\\] \\+ \\["+digit+"\\]\\{, \\} \\| \\$0\\$", "INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\
		\\(\
			\\[123456789\\] \\+ \\["+digit+"\\]\\{, \\} \\| \\$0\\$\
		\\) \\+ \\[Ll\\]", "LONG-INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\\$0\\$ \\+ \\[xX\\] \\+ \\["+hex+"\\]", "HEX-INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\\$0\\$ \\+ \\[xX\\] \\+ \\["+hex+"\\] \\+ \\[Ll\\]", "HEX-LONG-INT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\
		\\(\
			\\[123456789\\] \\+ \\["+digit+"\\]\\{, \\} \\|\
			\\$0\\$\
		\\)\\{0, 1\\} \\+ \\$.\\$ \\+ \\["+digit+"\\]\\{1, \\} \\+ \
		\\(\
			\\$e\\$ \\+ \\[-\\]\\{0, 1\\} \\+ \\[123456789\\] \\+ \\["+digit+"\\]\\{, \\}\
		\\)\\{0, 1\\}", "FLOAT-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\\$\"\\$ \\+ \\["+alphabet+"\\]\\{, \\} \\+ \\$\"\\$", "STRING-CONSTANT", true, constantLevel);
	cerr << "\\$\"\\$ \\+ \\["+alphabet+"\\]\\{, \\} \\+ \\$\"\\$" << endl;
	lex -> AddLexicalItem("\\$\'\\$ \\+ \\["+alphabet+"\\] \\+ \\$\'\\$", "CHAR-CONSTANT", true, constantLevel);
	lex -> AddLexicalItem("\\$NULL\\$", "POINTER-CONSTANT", true, constantLevel);

	// type ,class, declare
	lex -> AddLexicalItem("\\$void\\$", "VOID-TYPE", true, keywordLevel);
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
	lex -> AddLexicalItem("\\$switch\\$", "SWITCH", true, keywordLevel);
	lex -> AddLexicalItem("\\$case\\$", "CASE", true, keywordLevel);
	lex -> AddLexicalItem("\\$break\\$", "BREAK", true, keywordLevel);
	lex -> AddLexicalItem("\\$continue\\$", "CONTINUE", true, keywordLevel);
	lex -> AddLexicalItem("\\$return\\$", "RETURN", true, keywordLevel);
	lex -> AddLexicalItem("\\$typedef\\$", "TYPEDEF", true, keywordLevel);
	lex -> AddLexicalItem("\\$using\\$", "USING", true, keywordLevel);

	// operator and expression
	lex -> AddLexicalItem("\\$+\\$", "+", true);
	lex -> AddLexicalItem("\\$-\\$", "-", true);
	lex -> AddLexicalItem("\\$*\\$", "*", true);
	lex -> AddLexicalItem("\\$/\\$", "/", true);
	lex -> AddLexicalItem("\\$%\\$", "%", true);
	lex -> AddLexicalItem("\\$<\\$", "<", true);
	lex -> AddLexicalItem("\\$<=\\$", "<=", true);
	lex -> AddLexicalItem("\\$>=\\$", ">=", true);
	lex -> AddLexicalItem("\\$>\\$", ">", true);
	lex -> AddLexicalItem("\\$!=\\$", "!=", true);
	lex -> AddLexicalItem("\\$==\\$", "==", true);
	lex -> AddLexicalItem("\\$=\\$", "=", true);
	lex -> AddLexicalItem("\\$+=\\$", "+=", true);
	lex -> AddLexicalItem("\\$++\\$", "++", true);
	lex -> AddLexicalItem("\\$-=\\$", "-=", true);
	lex -> AddLexicalItem("\\$--\\$", "--", true);
	lex -> AddLexicalItem("\\$*=\\$", "*=", true);
	lex -> AddLexicalItem("\\$/=\\$", "/=", true);
	lex -> AddLexicalItem("\\$^=\\$", "^=", true);
	lex -> AddLexicalItem("\\$&=\\$", "&=", true);
	lex -> AddLexicalItem("\\$<<\\$", "<<", true);
	lex -> AddLexicalItem("\\$>>\\$", ">>", true);
	lex -> AddLexicalItem("\\$.\\$", ".", true);
	lex -> AddLexicalItem("\\$->\\$", "->", true);
	lex -> AddLexicalItem("\\$&\\$", "&", true);
	lex -> AddLexicalItem("\\$|\\$", "|", true);
	lex -> AddLexicalItem("\\$^\\$", "^", true);
	lex -> AddLexicalItem("\\$?\\$", "?", true);
	lex -> AddLexicalItem("\\$!\\$", "!", true);
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
	lex -> AddLexicalItem("\\$&&\\$", "&&", true);
	lex -> AddLexicalItem("\\$||\\$", "||", true);

	return lex;
}
