#include "FrontEnd.hpp"
#include "GrammerDefinition.hpp"
#include "LexDefinition.hpp"

string context;
char *buffer;

int readFile () {
	FILE * pFile;
	long lSize;
	size_t result;

	pFile = fopen ( "test.c" , "rb" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*(lSize + 1));
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	buffer[lSize] = '\0';

	/* the whole file is now loaded in the memory buffer. */

	// terminate
	fclose (pFile);
	return 0;
}

void input() {
    readFile();
    context = string(buffer);
}

vector<LexicalAnalyzer::LexicalItemInfo> lexResult;
void lex() {
    LexicalAnalyzer* lex = LexDefinition();
    LexicalAnalyzer::LexicalErrorInfo errInfo = lex -> LexicalAnalyze(context);
    if (errInfo.errorType == LexicalAnalyzer::LexicalErrorInfo::LexicalErrorType::MisMatch) {
        fprintf(stderr, "there is something wrong\n");
        exit(233);
    }
    lexResult = lex -> Result();
}

map<string, int> s2i;
map<int, string> i2s;
HumanGrammer hg;
LR::Grammer g;
void buildGrammer() {
    hg = GrammerDefinition();
    g = HG2G(hg, s2i, i2s);
    /* Debug */
    for (auto p: g.P) {
        cerr << p.lhs << " -> ";
        for (auto s: p.rhs) cerr << s << " ";
        cerr << endl;
    }
}

LR::String sentence;
void convertSentence() {
    sentence.clear();
    for (auto item: lexResult) {
        sentence.push_back(s2i[item.symbolType]);
    }
    sentence.push_back(s2i["-|"]);
    /* Debug */
    for (auto e: sentence) {
        printf("%d ", e);
    } puts("");
}

string handleSpecialCharacter(string s) {
    string ret;
    for (auto c: s) {
        if (c == '{' || c == '}' || c == '<' || c == '>' || c == '|' || c == '(' || c == ')') ret += '\\';
        ret += c;
    }
    return ret;
}

LR::ParseTree* tree;
vector<string> logContent;
void makeParseTree() {
    tree = Parse(g, sentence, 1);
    logContent.clear();
    for (int i = 0, lim = hg.I.size() + hg.T.size(); i < lim; ++i) logContent.push_back(handleSpecialCharacter(i2s[i]));
    /* print the parse tree */
    LR::ParseTreeLog(tree, logContent);
}

int main() {
    input();
    lex();
    buildGrammer();
    convertSentence();
    makeParseTree();
    return 0;
}
