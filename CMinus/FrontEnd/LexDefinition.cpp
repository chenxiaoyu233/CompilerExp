

void FrontEndImplement::lexDefinition() {
    /* set the aux charactor sets */
    string letter;
    for (int i = 0; i < 26; i++) letter += 'a' + i;
    for (int i = 0; i < 26; i++) letter += 'A' + i;
    string digit;
    for (int i = 0; i < 10; i++) digit += '0' + i;

    /* Lexical Definition */
    size_t keywordLevel = 10, constantLevel = 2, symbolLevel = 1;
    lex -> AddLexicalItem("\\[ \t\n\\]\\{1, \\}", "white space", false);
    /* Identifier */
    lex -> AddLexicalItem("\\["+letter+"\\]\\{1, \\} \\+ \\["+digit+"\\]\\{, \\}", "ID", true, symbolLevel);
    /* constant */
    lex -> AddLexicalItem("\\["+digit+"\\]\\{1, \\}", "NUM", true, constantLevel);
    lex -> AddLexicalItem("\\$true\\$", "true", true, constantLevel);
    lex -> AddLexicalItem("\\$false\\$", "false", true, constantLevel);
    lex -> AddLexicalItem("\\$\"\\$ \\+ \\["+letter+digit+" \t\n\\]\\{, \\} \\+ \\$\"\\$", "string", constantLevel);
    /* keyword */ 
	lex -> AddLexicalItem("\\$void\\$", "void", true, keywordLevel);
	lex -> AddLexicalItem("\\$int\\$", "int", true, keywordLevel);
	lex -> AddLexicalItem("\\$bool\\$", "bool", true, keywordLevel);
	lex -> AddLexicalItem("\\$string\\$", "bool", true, keywordLevel);
	lex -> AddLexicalItem("\\$if\\$", "if", true, keywordLevel);
	lex -> AddLexicalItem("\\$else\\$", "else", true, keywordLevel);
	lex -> AddLexicalItem("\\$while\\$", "while", true, keywordLevel);
    lex -> AddLexicalItem("\\$for\\$", "for", true, keywordLevel);
	lex -> AddLexicalItem("\\$return\\$", "return", true, keywordLevel);
    /* operator */
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
	lex -> AddLexicalItem("\\$-=\\$", "-=", true);
	lex -> AddLexicalItem("\\$&\\$", "&", true);
	lex -> AddLexicalItem("\\$|\\$", "|", true);
	lex -> AddLexicalItem("\\$^\\$", "^", true);
	lex -> AddLexicalItem("\\$!\\$", "!", true);
	lex -> AddLexicalItem("\\$(\\$", "(", true);
	lex -> AddLexicalItem("\\$)\\$", ")", true);
	lex -> AddLexicalItem("\\$[\\$", "[", true);
	lex -> AddLexicalItem("\\$]\\$", "]", true);
	lex -> AddLexicalItem("\\${\\$", "{", true);
	lex -> AddLexicalItem("\\$}\\$", "}", true);
	lex -> AddLexicalItem("\\$,\\$", ",", true);
	lex -> AddLexicalItem("\\$;\\$", ";", true);
}
