#include "FrontEndImplement.hpp"
#include "BackEndImplement.hpp"

extern string stop_at;
string file_name;
string context;
char *buffer;

int readFile () {
	FILE * pFile;
	long lSize;
	size_t result;

	pFile = fopen ( file_name.c_str() , "rb" );
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

string mcode;
MCodeBase* entry;

namespace BackEnd {
#include "DescribeTable.hpp"
    MCodeBase* CompleteIt(MCodeBase* ori) {
        MCodeBase* ret = new MCodeBase();
        ret -> include({"# init BP (R2)"});
        ret -> include({"LOD R2, STACK"});
        ret -> include({""});
        ret -> include({"# init global vars"});
        int cnt = 0;
        for (auto item: GDT) {
            //fprintf(stderr, "%s\n", item.name.c_str());
            ++cnt;
            ret -> include({"LOD R4,", item.initval});
            ret -> include({"STO (R2+" + to_string(8 * cnt) + "), R4"});
        }
        ret -> include({"LOD R2, R2+" + to_string(8 * GDT.size())});
        ret -> include({""});
        ret -> include({"# add entry"});
        ret -> include(entry); delete entry;
        entry = BackEndImplement("call <ret> = main\n").EndToEnd(1, "program");
        ret -> include(entry); delete entry;
        ret -> include({"# show return value"});
        ret -> include({"LOD R15, " + memVar("<ret>")});
        ret -> include({"OUT"});
        ret -> include({""});
        ret -> include({"# add Exit Port"});
        ret -> include({"END"});
        ret -> include({""});
        ret -> include(ori); delete ori;
        ret -> include({"# create STACK"});
        ret -> include({"STACK:"});
        ret -> include({""});
        return ret;
    }
}

int main(int argc, char **argv) {
    if (argc > 1) file_name = string(argv[1]);
    if (argc > 2) stop_at = string(argv[2]);
    input();
    FrontEndImplement Front(context, "FRONT_NODE.DUMP", "FRONT_TRANS.DUMP");
    MCodeBase* ret = Front.EndToEnd(1, "program");
    if (ret == NULL) return 0;
    if (stop_at == "mcode") {
        ret -> output(stdout);
        return 0;
    }
    if (stop_at == "mtree") {
        Front.LogParseTree();
        return 0;
    }
    mcode = ret -> toString();
    BackEnd::BackEndImplement Back(mcode, "BACK_NODE.DUMP", "BACK_TRANS.DUMP");
    entry = BackEnd::BackEndImplement("var <ret> 0 0\n").EndToEnd(1, "program");
    ret = Back.EndToEnd(1, "program");
    //Back.LogParseTree();
    if (ret == NULL) return 0;
    ret = BackEnd::CompleteIt(ret);
    ret -> output(stdout);
    //parser.LogParseTree();
    return 0;
}
