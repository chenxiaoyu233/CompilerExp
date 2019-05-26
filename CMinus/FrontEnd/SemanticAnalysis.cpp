#include "SemanticAnalysis.hpp"

MCodeBase* SemanticAnalysis(LR::ParseTree *rt, vector<MCodeBase*> &semantic, vector<LexicalAnalyzer::LexicalItemInfo> &lexResult, int &cnt) {
    if ((rt -> child).empty()) {
        /* we reach a leaf*/
        MCodeBase *cur = new MCodeBase();
        if (rt -> pid == -1) {
            /* think carefully on empty string */
            (cur -> code).push_back({lexResult[cnt].content});
            /* maintain the range of this node */
            cur -> begin = lexResult[cnt].begin;
            cur -> end = lexResult[cnt].end;
            /* add the counter for the next leaf */
            ++ cnt;
        }
        return cur;
    }
   
    MCodeBase* ret = new MCodeBase();

    /* enumerate on childs */
    vector<MCodeBase*> vec; vec.clear();
    for (auto ch: rt -> child) {
        MCodeBase* cur = SemanticAnalysis(ch, semantic, lexResult, cnt);
        if (ret -> begin == -1) ret -> begin = cur -> begin;
        if (cur -> end != -1) ret -> end = cur -> end;
        vec.push_back(cur);
    }
    
    assert(vec.size() == rt -> child.size());
    
    /* generate the MCode for this node */
    semantic[rt -> pid] -> child.clear();
    for (auto ch: vec)
        semantic[rt -> pid] -> child.push_back(ch);

    /* generate MCode */
    semantic[rt -> pid] -> generate(ret);

    /* freechlid node */
    for (auto p: vec) delete p;

    return ret;
}
