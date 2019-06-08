#include "DescribeTable.hpp"

void BackEndImplement::grammerDefinition() {
    PE("program -> statement-declare", ret -> include(child[0]););
    PE("statement-declare ->", /* do nothing here */);
    PE("statement-declare -> statement-list", ret -> include(child[0]););
    PE("statement-list -> statement-list statement",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("statement-list -> statement", ret -> include(child[0]););
    PE("statement -> simple-statement", ret -> include(child[0]););
    PE("statement -> scope-statement", ret -> include(child[0]););
    PE("statement -> function-statement", ret -> include(child[0]););
    PE("function-statement -> function-head statement-declare end",
        ret -> include(child[0]);
        ret -> include(child[1]);
        if (!LDT.front().empty()) { /* has applied new var */
            ret -> include({"LOD R2, R2-" + to_string(BP-LDT.front().front().addr+8)});
            BP = LDT.front().front().addr - 8;
        }
        ret -> include({"# end function"});
        ret -> include({""});
        closeScope();
    );
    PE("function-head -> f-head formal-declare",
        ret -> include(child[0]);
        ret -> include(child[1]);
        addVar("ret-ip-addr");
        ret -> include({"LOD R2, R2+8"});
        ret -> include({""});
    );
    PE("f-head -> label ID begin",
        ret -> include({"# begin function"});
        ret -> include({ch(1) + ":"});
        startScope();
        addVar("ret-data-addr");
        ret -> include({"LOD R2, R2+8"});
    );
    PE("scope-statement -> scope-head statement-declare #end",
        ret -> include(child[0]);
        ret -> include(child[1]);
        if (!LDT.front().empty()) { /* has applied new var */
            ret -> include({"LOD R2, R2-" + to_string(BP-LDT.front().front().addr+8)});
            BP = LDT.front().front().addr - 8;
        }
        ret -> include({"# close scope"});
        ret -> include({""});
        closeScope();
    );
    PE("scope-head -> #begin",
        ret -> include({"# begin scope"});
        startScope();
    );
    PE("formal-declare -> formal-list",
        int size = (child[0] -> code).size();
        for (auto item: child[0] -> code) {
            if (isDuplicate(item[0])) {
                fprintf(stderr, "duplicate\n");
                exit(233);
            }
            addVar(item[0]);
        }
        ret -> include({"LOD R2, R2+" + to_string(size*8)});
    );
    PE("formal-declare ->", /* do nothing here */);
    PE("formal-list -> formal-list formal-line",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("formal-list -> formal-line", ret -> include(child[0]););
    PE("formal-line -> formal ID", ret -> include(child[1]););
    PE("simple-statement -> var ID NUM NUM",
        ret -> include({"#", ch(0), ch(1), ch(2), ch(3)});
        if (isDuplicate(ch(1))) {
            fprintf(stderr, "duplicate\n");
            exit(233);
        }
        addVar(ch(1));
        if (!LDT.empty()) {
            ret -> include({"LOD R2, R2+8"});
            ret -> include({"LOD R4,", ch(3)});
            ret -> include({"STO " + memVar(ch(1)) + ", R4"});
        } else {
            GDT.back().initval = ch(3);
        }
        ret -> include({""});
    );
    PE("simple-statement -> var ID",
        ret -> include({"#", ch(0), ch(1)});
        if (isDuplicate(ch(1))) {
            fprintf(stderr, "duplicate\n");
            exit(233);
        }
        addVar(ch(1));
        if (!LDT.empty()) {
            ret -> include({"LOD R2, R2+8"});
            ret -> include({""});
        }
    );
    PE("simple-statement -> ID = ID",
        ret -> include({"#", ch(0), ch(1), ch(2)});
        ret -> include({"LOD R4, " + memVar(ch(2))});
        ret -> include({"STO "+ memVar(ch(0)) +", R4"});
        ret -> include({""});
    );
    PE("simple-statement -> ID = NUM",
        ret -> include({"#", ch(0), ch(1), ch(2)});
        ret -> include({"LOD R4, " + ch(2)});
        ret -> include({"STO "+ memVar(ch(0)) +", R4"});
        ret -> include({""});
    );
    PE("simple-statement -> ID = ID op ID",
        ret -> include({"#", ch(0), ch(1), ch(2), ch(3), ch(4)});
        ret -> include({"LOD R4, " + memVar(ch(2))});
        ret -> include({"LOD R5, " + memVar(ch(4))});
        ret -> include({ch(3) + " R4, R5"});
        ret -> include({"STO " + memVar(ch(0)) + ", R4"});
        ret -> include({""});
    );
    PE("simple-statement -> ID = u-op ID",
        ret -> include({"#", ch(0), ch(1), ch(2), ch(3)});
        ret -> include({"LOD R4, 0"});
        ret -> include({"LOD R5, " + memVar(ch(3))});
        ret -> include({ch(2) + "R4, R5"});
        ret -> include({"STO " + memVar(ch(0)) + ", R4"});
        ret -> include({""});
    );
    PE("simple-statement -> ID = REG ( NUM )",
        ret -> include({"#", ch(0), ch(1), ch(2), ch(3), ch(4), ch(5)});
        ret -> include({"STO " + memVar(ch(0)) + ", R" + ch(4)});
        ret -> include({""});
    );
    PE("simple-statement -> ID = TST ID",
        ret -> include({"#", ch(0), ch(1), ch(2), ch(3)});
        ret -> include({"LOD R4, " + memVar(ch(3))});
        ret -> include({"TST R4"});
        ret -> include({"STO " + memVar(ch(0)) + ", R0"});
        ret -> include({""});
    );
    PE("simple-statement -> label ID",
        ret -> include({"#", ch(0), ch(1)});
        ret -> include({ch(1) + ":"});
        ret -> include({""});
    );
    PE("simple-statement -> goto ID",
        ret -> include({"#", ch(0), ch(1)});
        ret -> include({"JMP " + ch(1)});
        ret -> include({""});
    );
    PE("simple-statement -> if ID goto ID",
        ret -> include({"#", ch(0), ch(1), ch(2), ch(3)});
        ret -> include({"LOD R4, " + memVar(ch(1))});
        ret -> include({"TST R4"});
        ret -> include({"JGZ " + ch(3)});
        ret -> include({"JLZ " + ch(3)});
        ret -> include({""});
    );
    PE("simple-statement -> ifz ID goto ID",
        ret -> include({"#", ch(0), ch(1), ch(2), ch(3)});
        ret -> include({"LOD R4, " + memVar(ch(1))});
        ret -> include({"TST R4"});
        ret -> include({"JEZ " + ch(3)});
        ret -> include({""});
    );
    PE("simple-statement -> out ID",
        ret -> include({"#", ch(0), ch(1)});
        ret -> include({"LOD R15, " + memVar(ch(1))});
        ret -> include({"OUT"});
        ret -> include({""});
    );
    PE("simple-statement -> return ID",
        ret -> include({"#", ch(0), ch(1)});
        ret -> include({"LOD R4, " + memVar(ch(1))});
        ret -> include({"STO " + memVar("ret-data-addr") + ", R4"});
        ret -> include({"LOD R3, " + memVar("ret-ip-addr")});
        bool isGlobal = false;
        DescribeTableItem v = find("ret-data-addr", isGlobal);
        ret -> include({"LOD R2, R2-" + to_string(BP - v.addr)});
        ret -> include({"JMP R3"});
        ret -> include({""});
    );
    PE("simple-statement -> function-call",ret -> include(child[0]););
    PE("function-call -> actual-param call ID = ID",
        /* set the return value */
        int base = BP;
        BP += 8;
        ret -> include({"# set return value"});
        ret -> include({"LOD R2, R2 + 8"});
        ret -> include({""});
        /* set the actual parameter */
        for (auto item: child[0] -> code) {
            BP += 8;
            ret -> include({"# formal", item[0]});
            ret -> include({"LOD R2, R2 + 8"});
            ret -> include({"LOD R4, " + memVar(item[0])});
            ret -> include({"STO (R2), R4"});
            ret -> include({""});
        }
        /* store the return ip */
        ret -> include({"#", ch(1), ch(2), ch(3), ch(4)});
        BP += 8;
        ret -> include({"# set return address"});
        ret -> include({"LOD R2, R2+8"});
        ret -> include({"STO (R2), R1+24"});
        ret -> include({"LOD R2, R2-" + to_string(BP-base)});
        ret -> include({"JMP " + ch(4)});
        BP = base + 8;
        ret -> include({"LOD R4, (R2)"});
        ret -> include({"STO " + memVar(ch(2)) + ", R4"});
        ret -> include({"LOD R2, R2-8"});
        BP = base;
        ret -> include({""});
    );
    PE("actual-param -> actual-list", ret -> include(child[0]););
    PE("actual-param ->", /* do nothing here */);
    PE("actual-list -> actual-list actual-line",
        ret -> include(child[0]);
        ret -> include(child[1]);
    );
    PE("actual-list -> actual-line", ret -> include(child[0]););
    PE("actual-line -> actual ID", ret -> include(child[1]););
    PE("op -> +", ret -> include({"ADD"}););
    PE("op -> -", ret -> include({"SUB"}););
    PE("op -> *", ret -> include({"MUL"}););
    PE("op -> /", ret -> include({"DIV"}););
    PE("u-op -> -", ret -> include({"SUB"}););
    PE("u-op -> +", ret -> include({"ADD"}););
}
