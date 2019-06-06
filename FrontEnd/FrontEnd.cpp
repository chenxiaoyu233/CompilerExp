#include "FrontEnd.hpp"

void HumanGrammer::AddProduction(string s) {
    stringstream ss(s);
    string lhs, arrow;
    ss >> lhs >> arrow;
    vector<string> rhs; rhs.clear();
    while(!ss.eof()) {
        string tail; ss >> tail;
        if (tail == "") continue;
        if (tail == "|") {
            P.push_back(HumanProduction{lhs, rhs});
            rhs.clear();
        } else if (tail == "\\|") {
            rhs.push_back("|");
        } else {
            rhs.push_back(tail);
        }
    }
    /* push back the last production */
    P.push_back(HumanProduction{lhs, rhs});
}

/* build the set T and set I*/
void HumanGrammer::BuildIT() {
    I.clear(); T.clear();
    for (auto pd: P) if (!I.count(pd.lhs)) I.insert(pd.lhs);
    for (auto pd: P) for (auto s: pd.rhs) if (!I.count(s) && !T.count(s)) T.insert(s);
}

/* build cE using a brute force search */
void HumanGrammer::BuildcE() {
    bool flag = true;
    while (flag) {
        flag = false;
        for (auto p: P) {
            /* case 1: rhs is an empty string */
            if (p.rhs.empty() && !cE.count(p.lhs)) { flag = true; cE.insert(p.lhs); }
            /* case 2: rhs is made up by some internal symbol x1, x2, ..., xn, and all of them are in set cE */
            bool rhsCouldEmpty = true;
            for (auto s: p.rhs) if (!cE.count(s)) { rhsCouldEmpty = false; break; }
            if (rhsCouldEmpty && !cE.count(p.lhs)) { flag = true; cE.insert(p.lhs); }
        }
    }
}

HumanGrammer::HumanGrammer() {
    /* Initialize */
    I.clear(); T.clear(); cE.clear(); P.clear();
}

HumanGrammer::HumanGrammer(int k, string start) {
    /* Initialize */
    I.clear(); T.clear(); cE.clear(); P.clear();
    /* fix grammer for LR(k) */
    vector<string> rhs; rhs.clear();
    rhs.push_back(start);
    for (int i = 0; i < k; ++i) rhs.push_back("-|");
    P.push_back(HumanProduction{"S", rhs});
}

void FrontEnd::indexSymbols(HumanGrammer hg, map<string, int> &s2i, map<int, string> &i2s) {
    /* Initialize */
    s2i.clear(); i2s.clear(); 
    int cnt = 0; // leave 0 for S, S must be 0
    s2i["S"] = 0; s2i["-|"] = -1;
    /* index */
    for (auto s: hg.I) if(s.size() != 0 && s != "S") s2i[s] = ++cnt;
    for (auto s: hg.T) if(s.size() != 0) s2i[s] = ++cnt;
    /* create inverse */
    for (auto p: s2i) i2s[p.second] = p.first;
    /* Debug */
}

FrontEnd::FrontEnd(string context):context(context) {
    s2i.clear(); i2s.clear();
    tree = NULL;
    lex = NULL;
}

FrontEnd::~FrontEnd() {
    if (lex != NULL) delete lex;
    if (tree != NULL) deleteTree(tree);
}

void FrontEnd::deleteTree(LR::ParseTree *rt) {
    for (auto son: rt -> child) deleteTree(son);
    delete rt;
}


/* Convert a HumanGrammer hg to a Grammer which is recognized by the parser */
LR::Grammer FrontEnd::HG2G(HumanGrammer hg, map<string, int> &s2i, map<int, string> &i2s) {
    /* Initialize */
    LR::Grammer g;
    g.P.clear(); g.I.clear(); g.cE.clear();
    indexSymbols(hg, s2i, i2s);
    /* fill Grammer g */
    /* build product */
    for (auto p: hg.P) {
        LR::Character lhs = s2i[p.lhs];
        vector<LR::Character> rhs; rhs.clear();
        for (auto s: p.rhs) rhs.push_back(s2i[s]);
        g.P.push_back(LR::Production{lhs, rhs});
    }
    /* build Internal Set */
    for (auto s: hg.I) if (s.size() != 0) g.I.insert(s2i[s]);
    /* build Terminal Set */
    for (auto s: hg.T) if (s.size() != 0) g.T.insert(s2i[s]);
    /* build Could Empty Set */
    for (auto s: hg.cE) if (s.size() != 0) g.cE.insert(s2i[s]);

    return g;
}

void FrontEnd::LexDefinition() {
    lex = new LexicalAnalyzer();
    this -> lexDefinition();
}

void FrontEnd:: LexProcess() {
    LexicalAnalyzer::LexicalErrorInfo errInfo = lex -> LexicalAnalyze(context);
    if (errInfo.errorType == LexicalAnalyzer::LexicalErrorInfo::LexicalErrorType::NoError) {
        lexResult = lex -> Result();
    } else {
        this -> lexErrorHandler(errInfo);
        exit(0);
    }
}

void FrontEnd::GrammerDefinition() {
    /* hold place for Production S */
    semantic.push_back(new MCodeBase());
    this -> grammerDefinition();
    hg.BuildIT();
    hg.BuildcE();
    g = HG2G(hg, s2i, i2s);
}

MCodeBase* FrontEnd::SemanticAnalysis(LR::ParseTree *rt, int &cnt, int &errorCnt, int &warnCnt) {
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
        MCodeBase* cur = SemanticAnalysis(ch, cnt, errorCnt, warnCnt);
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
    semantic[rt -> pid] -> generate(ret, context);
    /* collect the error and warn number from this node */
    errorCnt += ret -> errorCnt;
    warnCnt += ret -> warnCnt;
    
    
    /* freechlid node */
    for (auto p: vec) delete p;
    
    return ret;
}

bool FrontEnd::GrammerProcess() {
    bool flag = true;
    // convert the lexReslut to a sentence that could be read by LR algorithm
    sentence.clear();
    for (auto item: lexResult) {
        if (!s2i.count(item.symbolType)) {
            fprintf(stderr, "the lex symbol \'%s\' you define dose not match any grammer symbol you define\n", item.symbolType.c_str());
            assert(s2i.count(item.symbolType));
        }
        sentence.push_back(s2i[item.symbolType]);
    }
    for (int i = 0; i < k; ++i) sentence.push_back(s2i["-|"]);
    
    // generate the parse tree
    int errorAt = -233;
    tree = Parse(g, sentence, k, errorAt);
    if (errorAt != -233) { /* error happens */
        flag &= this -> grammerErrorHandler(errorAt);
    }
    
    // generate the content for logger
    logContent.clear();
    for (int i = 0, lim = hg.I.size() + hg.T.size(); i < lim; ++i)
        logContent.push_back(handleSpecialCharacter(i2s[i]));
    
    return flag;
}

bool FrontEnd::grammerErrorHandler(int errorAt) {
    ErrorReport(context).Report(
        "error", "Syntax Error",
        lexResult[errorAt].begin, lexResult[errorAt].end
    );
    exit(233);
    return false;
}

string FrontEnd::handleSpecialCharacter(string s) {
    string ret;
    for (auto c: s) {
        if (c == '{' || c == '}' || c == '<' || c == '>' || c == '|' || c == '(' || c == ')' || c == '[' || c == ']') ret += '\\';
        ret += c;
    }
    return ret;
}

void FrontEnd::LogParseTree() {
    /* generate debug content for lex content */
    vector<string> lexContent; lexContent.clear();
    for (auto item: lexResult)
        lexContent.push_back(handleSpecialCharacter(item.content));
    /* print the parse tree */
    LR::ParseTreeLog(tree, logContent, &lexContent);
}

void FrontEnd::LogDFA() {
    lex -> LogDfa();
}

MCodeBase* FrontEnd::EndToEnd(int k, string start) {
    this -> k = k;
    hg = HumanGrammer(k, start);
    LexDefinition();
    LexProcess();
    if(!AfterLex()) return NULL;
    GrammerDefinition();
    if(!GrammerProcess()) return NULL;
    if(!AfterGrammer()) return NULL;
    int cnt = 0;
    int errorCnt = 0, warnCnt = 0;;
    MCodeBase* ret = SemanticAnalysis(tree, cnt, errorCnt, warnCnt);
    if (errorCnt > 0 || warnCnt > 0) {
        fprintf(stderr, "%d errors generated, %d warnings generated\n", errorCnt, warnCnt);
        if (errorCnt > 0) return NULL;
    }
    return ret;
}
