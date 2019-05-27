#include "FrontEndImplement.hpp"

extern string stop_at;

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

int main(int argc, char **argv) {
    if (argc > 1) stop_at = string(argv[1]);
    input();
    FrontEndImplement parser(context);
    MCodeBase* ret = parser.EndToEnd(1, "program");
    if (ret != NULL) ret -> output(stderr);
    //parser.LogParseTree();
    return 0;
}
