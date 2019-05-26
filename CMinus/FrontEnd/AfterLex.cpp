

bool FrontEndImplement::AfterLex() {
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
        return false;
    }
    return true;
}
