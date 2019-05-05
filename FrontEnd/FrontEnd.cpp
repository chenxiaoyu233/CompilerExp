#include "FrontEnd.hpp"

void HumanGrammer::AddProduction(string s) {
    stringstream ss(s);
    string lhs, arrow;
    ss >> lhs >> arrow;
    vector<string> rhs; rhs.clear();
    set<string> total;
    while(!ss.eof()) {
        string tail; ss >> tail;
        if (tail == "|") {
            P.push_back(HumanProduction{lhs, rhs});
            if (!I.count(lhs)) I.insert(lhs);
            rhs.clear();
        } else if (tail == "\\|") {
            rhs.push_back("|");
            if (!total.count("|")) total.insert("|");
        } else {
            rhs.push_back(tail);
            if (!total.count(tail)) total.insert(tail);
        }
    }
    /* push back the last production */
    P.push_back(HumanProduction{lhs, rhs});
    if (!I.count(lhs)) I.insert(lhs);
    /* calc the set T */
    for (auto s: total) if (!I.count(s) && !T.count(s)) T.insert(s);
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

void indexSymbols(HumanGrammer hg, map<string, int> &s2i, map<int, string> &i2s) {
    /* Initialize */
    s2i.clear(); i2s.clear(); 
    int cnt = 1; // leave 0 for S, S must be 0
    s2i["S"] = 0; s2i["-|"] = -1;
    /* index */
    for (auto s: hg.I) if(s.size() != 0) s2i[s] = ++cnt;
    for (auto s: hg.T) if(s.size() != 0) s2i[s] = ++cnt;
    /* create inverse */
    for (auto p: s2i) i2s[p.second] = p.first;
    /* Debug */
}

/* Convert a HumanGrammer hg to a Grammer which is recognized by the parser */
LR::Grammer HG2G(HumanGrammer hg, map<string, int> &s2i, map<int, string> &i2s) {
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

