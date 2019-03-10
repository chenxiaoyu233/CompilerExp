// this is a stub module for Lexical Analyzer
#include "common.h"
#include "LexicalAnalyzer.hpp"

#include "LexicalDefinition.cpp"

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
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	/* the whole file is now loaded in the memory buffer. */

	// terminate
	fclose (pFile);
	return 0;
}

int main() {
	LexicalAnalyzer *lex = LexicalDefinition();
	readFile();
	LexicalAnalyze::LexicalErrorInfo errInfo = lex -> LexicalAnalyze(string(buffer));
	if (errInfo.errorType != LexicalAnalyzer::LexicalErrorInfo::LexicalErrorType::NoError) {
		fprintf(stderr, "something wrong\n");
	}
	vector<LexicalItemInfo> items = lex -> Result();
	for (auto item: items) {
		printf("(%s, %s)\n", item.content.c_str(), item.symbolType.c_str());
	}
	return 0;
}
