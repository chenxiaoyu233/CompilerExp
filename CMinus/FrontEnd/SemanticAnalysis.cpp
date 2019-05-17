#include "SemanticAnalysis.hpp"

MCodeBase* SemanticAnalysis(LR::ParseTree *rt, vector<MCodeBase*> &semantic, vector<LexicalAnalyzer::LexicalItemInfo> &lexResult, int &cnt) {
    if ((rt -> child).empty()) {
        /* we reach a leaf*/
        MCodeBase *cur = new MCodeBase();
        if (rt -> pid == -1) {
            /* think carefully on empty string */
            (cur -> code).push_back(MCodeTuple("", lexResult[cnt].content, "", ""));
            /* add the counter for the next leaf */
            ++ cnt;
        }
        return cur;
    }
   
    /* enumerate on childs */
    vector<MCodeBase*> vec; vec.clear();
    for (auto ch: rt -> child) {
        vec.push_back(SemanticAnalysis(ch, semantic, lexResult, cnt));
    }
    
    /* generate the MCode for this node */
    semantic[rt -> pid] -> child.clear();
    for (auto ch: vec)
        semantic[rt -> pid] -> child.push_back(ch);
    return semantic[rt -> pid] -> generate();
}
