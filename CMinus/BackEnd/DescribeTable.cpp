#include "DescribeTable.hpp"

namespace BackEnd {
    list<DescribeTableItem> GDT;
    list<list<DescribeTableItem> > LDT;
    int BP;

    void startScope() { LDT.push_front(list<DescribeTableItem>()); }
    void closeScope() { LDT.pop_front(); }
    bool isDuplicate(string name) {
        if (LDT.empty()) {
            for (auto item: GDT) if(item.name == name) return true;
        } else {
            for (auto item: LDT.front()) if (item.name == name) return true;
        }
        return false;
    }
    bool exists(string name) {
        for (auto block: LDT) for (auto item: block) if (item.name == name) return true;
        for (auto item: GDT) if (item.name == name) return true;
        return false;
    }

    DescribeTableItem find(string name) {
        for (auto block: LDT) for (auto item: block) if (item.name == name) return item;
        for (auto item: GDT) if (item.name == name) return item;
    }

    void addVar(string name, int addr) {
        if(addr == -1) { BP += 8; addr = BP; }
        if(LDT.empty()) GDT.push_back({name, addr});
        else LDT.front().push_back({name, addr});
    }

    string memVar(string name) {
        if (!exists(name)) {
            fprintf(stderr, "var %s does not exist\n", name.c_str());
            exit(233);
        }
        DescribeTableItem v = find(name);
        return "(R2-" + to_string(BP - v.addr) + ")";
    }

}
