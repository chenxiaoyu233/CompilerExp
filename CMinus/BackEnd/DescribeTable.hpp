#ifndef __DESCRIBE_TABLE_H__
#define __DESCRIBE_TABLE_H__

#include "common.hpp"
struct DescribeTableItem {
    string name;
    int addr;
};

extern int BP;

void startScope();
void closeScope();
bool isDuplicate(string name);
bool exists(string name);
DescribeTableItem find(string name);
void addVar(string name, int addr = -1);
string memVar(string name);

#endif
