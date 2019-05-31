#include "DescribeTable.hpp"

void BackEndImplement::grammerDefinition() {
    PE("program -> statement-list", ret -> include(child[0]););
    PE("statement-list -> statement-list statement",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("statement-list -> statement", ret -> include(child[0]););
    PE("statement -> simple-statement", ret -> include(child[0]););
    PE("statement -> scope-statement", ret -> include(child[0]););
    PE("scope-statement -> scope-head formal-declare statement-list end",
        ret -> include(child[1]);
        ret -> include(child[2]);
        closeScope();
    );
    PE("scope-head -> begin",
        startScope();
    );
    PE("formal-declare -> formal-list",
        int size = (child[0] -> code).size();
        addVar("ret-data-addr");
        for (auto item: child[0] -> code) {
            if (isDuplicate(item[0])) {
                fprintf(stderr, "duplicate\n");
                exit(233);
            }
            addVar(item[0]);
        }
        addVar("ret-ip-addr");
        ret -> include({"LOD R2, R2+" + to_string((2+size)*4)});
    );
    PE("formal-list -> formal-list formal-line",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("formal-list ->", /* do nothing here */);
    PE("formal-line -> formal ID", ret -> include(child[1]););
    PE("simple-statement -> var ID",
        if (isDuplicate(ch(1))) {
            fprintf(stderr, "duplicate\n");
            exit(233);
        }
        addVar(ch(1));
        ret -> include({"LOD R2, R2+4"});
    );
    PE("simple-statement -> ID = ID",
        ret -> include({"LOD R4, " + memVar(ch(1))});
        ret -> include({"STO "+ memVar(ch(0)) +", R4"});
    );
    PE("simple-statement -> ID = NUM",
        ret -> include({"STO "+ memVar(ch(0)) +", "+ ch(1)});
    );
    PE("simple-statement -> ID = ID op ID",
        ret -> include({"LOD R4, " + memVar(ch(2))});
        ret -> include({"LOD R5, " + memVar(ch(4))});
        ret -> include({ch(3) + " R4, R5"});
        ret -> include({"STO " + memVar(ch(0)) + ", R4"});
    );
    PE("simple-statement -> ID = u-op ID",
        ret -> include({"LOD R4, 0"});
        ret -> include({"LOD R5, " + memVar(ch(3))});
        ret -> include({ch(2) + "R4, R5"});
        ret -> include({"STO " + memVar(ch(0)) + ", R4"});
    );
    PE("simple-statement -> label ID",
        ret -> include({ch(1) + ":"});
    );
    PE("simple-statement -> goto ID",
        ret -> include({"JMP " + ch(1)});
    );
    PE("simple-statement -> if ID goto ID",
        ret -> include({"LOD R4, " + memVar(ch(1))});
        ret -> include({"TST R4"});
        ret -> include({"JNZ " + ch(3)});
    );
    PE("simple-statement -> ifz ID goto ID",
        ret -> include({"LOD R4, " + memVar(ch(1))});
        ret -> include({"TST R4"});
        ret -> include({"JEZ " + ch(3)});
    );
    PE("simple-statement -> return ID",
        ret -> include({"LOD R4, " + memVar(ch(1))});
        ret -> include({"STO " + memVar("ret-data-addr") + ", R4"});
        ret -> include({"LOD R3, " + memVar("ret-ip-addr")});
        DescribeTableItem v = find("ret-data-addr");
        ret -> include({"LOD R2, R2-" + to_string(BP - v.addr)});
        BP = v.addr;
        ret -> include({"JMP R3"});
    );
    PE("simple-statement -> function-call", ret -> include(child[0]););
    PE("function-call -> actual-list ID = call ID",
        /* set the return value */
        int base = BP;
        BP += 4;
        ret -> include({"LOD R2, R2 + 4"});
        /* set the actual parameter */
        for (auto item: child[0] -> code) {
            BP += 4;
            ret -> include({"LOD R2, R2 + 4"});
            ret -> include({"LOD R4, " + memVar(item[0])});
            ret -> include({"STO (R2), R4"});
        }
        /* store the return ip */
        BP += 4;
        ret -> include({"LOD R2, R2+4"});
        ret -> include({"STO (R2), R1+12"});
        ret -> include({"LOD R2, R2-" + to_string(BP-base)}); BP = base;
        ret -> include({"JMP " + ch(4)});
        ret -> include({"LOD R4, " + to_string(BP-base+4)});
        ret -> include({"STO " + memVar(ch(1)) + ", R4"});
        ret -> include({"LOD R2, R2-" + to_string(BP-base)}); BP = base;
    );
    PE("actual-list -> actual-list actual-line",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("actual-list ->", /* do nothing here */);
    PE("actual-line -> actual ID", ret -> include(child[1]););
    PE("op -> +", ret -> include({"ADD"}););
    PE("op -> -", ret -> include({"SUB"}););
    PE("op -> *", ret -> include({"MUL"}););
    PE("op -> /", ret -> include({"DIV"}););
    PE("u-op -> -", ret -> include({"SUB"}););
    PE("u-op -> +", ret -> include({"ADD"}););
}
