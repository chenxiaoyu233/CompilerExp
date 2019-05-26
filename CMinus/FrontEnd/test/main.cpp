#include "FrontEnd.hpp"
#include "GrammerDefinition.hpp"
#include "LexDefinition.hpp"
#include "SemanticAnalysis.hpp"

string stop_at;

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
        fprintf(stderr, "there is something wrong at pos %zu\n", errInfo.position);
        ErrorReport(context).ReportAtPointInLine(errInfo.position);
        exit(233);
    }
    lexResult = lex -> Result();
    if (stop_at == "lex") {
        /* return the result from the lex stage, and halt */
        /*symbol table to*/
        char buffer[10005];
        char *pt = buffer;
        unordered_map<string, char*> ST; 
        for (auto item: lexResult) {
            //ST[item.content] = strdup(item.content.c_str());
            ST[item.content] = pt;
            for (auto c: item.content) *(pt++) = c;
            *(pt++) = '\0';
        }
        printf("%10s %10s %10s %10s %10s\n", "content", "sym-type", "begin", "end", "addr");
        printf("%10s %10s %10s %10s %10s\n", "-------", "--------", "-----", "---", "----");
        for (auto item: lexResult) {
            printf("%10s %10s %10zu %10zu %10p\n", item.content.c_str(), item.symbolType.c_str(), item.begin, item.end, ST[item.content]);
        }
        printf("Symbol Table\n");
        printf("%10s %20s %10s\n", "symbol", "addr", "content");
        printf("%10s %20s %10s\n", "------", "----", "-------");
        for (auto pr: ST) {
            printf("%10s %20p %10s\n", pr.first.c_str(), pr.second, pr.second);
        }
        exit(0);
    }
}

map<string, int> s2i;
map<int, string> i2s;
HumanGrammer hg;
LR::Grammer g;
void buildGrammer() {
    hg = GrammerDefinition();
    g = HG2G(hg, s2i, i2s);
}

LR::String sentence;
void convertSentence() {
    sentence.clear();
    for (auto item: lexResult) {
        sentence.push_back(s2i[item.symbolType]);
    }
    sentence.push_back(s2i["-|"]);
}

string handleSpecialCharacter(string s) {
    string ret;
    for (auto c: s) {
        if (c == '{' || c == '}' || c == '<' || c == '>' || c == '|' || c == '(' || c == ')' || c == '[' || c == ']') ret += '\\';
        ret += c;
    }
    return ret;
}

LR::ParseTree* tree;
vector<string> logContent;
void makeParseTree() {
    tree = Parse(g, sentence, 1);
    logContent.clear();
    for (int i = 0, lim = hg.I.size() + hg.T.size(); i < lim; ++i) 
        logContent.push_back(handleSpecialCharacter(i2s[i]));
    /* print the parse tree */
    LR::ParseTreeLog(tree, logContent);
}

void semanticAnalysis() {
    int cnt = 0;
    MCodeBase* ret = SemanticAnalysis(tree, semantic, lexResult, cnt);
    for (auto tp: ret -> code) {
        for (auto s: tp) {
            printf("%s ", s.c_str());
        } printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc > 1) stop_at = string(argv[1]);
    input();
    lex();
    buildGrammer();
    convertSentence();
    makeParseTree();
    semanticAnalysis();
    return 0;
}
