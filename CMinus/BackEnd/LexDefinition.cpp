

void BackEndImplement::lexDefinition() {
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
    lex -> AddLexicalItem(
        "\\(\\["+letter+"\\]\\{1, \\} \\+ \\["+digit+"\\]\\{, \\}\\) \\| "
        "\\( \\$<\\$ \\["+letter+"\\]\\{1, \\} \\+ \\["+digit+"\\]\\{, \\} \\$>\\$ \\)", 
    "ID", true, symbolLevel);
    /* constant */
    lex -> AddLexicalItem("\\["+digit+"\\]\\{1, \\}", "NUM", true, constantLevel);
    //lex -> AddLexicalItem("\\$\"\\$ \\+ \\["+letter+digit+" \t\n\\]\\{, \\} \\+ \\$\"\\$", "string", constantLevel);
    /* keyword */ 
	lex -> AddLexicalItem("\\$var\\$", "var", true, keywordLevel);
	lex -> AddLexicalItem("\\$label\\$", "label", true, keywordLevel);
	lex -> AddLexicalItem("\\$formal\\$", "formal", true, keywordLevel);
	lex -> AddLexicalItem("\\$actual\\$", "actual", true, keywordLevel);
	lex -> AddLexicalItem("\\$begin\\$ \\| \\$#begin\\$", "begin", true, keywordLevel);
	lex -> AddLexicalItem("\\$end\\$ \\| \\$#begin\\$", "end", true, keywordLevel);
	lex -> AddLexicalItem("\\$call\\$", "call", true, keywordLevel);
	lex -> AddLexicalItem("\\$return\\$", "return", true, keywordLevel);
	lex -> AddLexicalItem("\\$if\\$", "if", true, keywordLevel);
	lex -> AddLexicalItem("\\$ifz\\$", "ifz", true, keywordLevel);
	lex -> AddLexicalItem("\\$goto\\$", "goto", true, keywordLevel);
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
	lex -> AddLexicalItem("\\$&\\$", "&", true);
	lex -> AddLexicalItem("\\$|\\$", "|", true);
	lex -> AddLexicalItem("\\$^\\$", "^", true);
	lex -> AddLexicalItem("\\$!\\$", "!", true);
}
