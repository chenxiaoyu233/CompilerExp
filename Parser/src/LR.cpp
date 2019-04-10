
typedef int Character;
typedef list<Character> String;
struct StrCmp {
    bool operator () (const String &a, const String &b) {
        if (a.size() != b.size()) return a.size() < b.size();
        for (size_t i = 0, r = min(a.size(), b.size()); i < r; ++i) {
            if (a[i] != b[i]) return a[i] < b[i];
        }
        return false;
    }
};
typedef set<String, StrCmp> FollowSet;
struct ParseTree { Character X; vector<ParseTree*> chlid; };
struct State { int id, pos; String follow; };
struct StateCmp {
    bool operator () (const State &a, const State &b) {
        StrCmp sp;
        if (a.id == b.id) {
            if (a.pos == b.pos) {
                return sp(a.follow, b.follow);
            }
            return a.pos < b.pos;
        }
        return a.id < b.id;
    }
}
typedef set<State, StateCmp> StateSet;
struct Production { Character lhs; String rhs; };
struct Grammer { vector<Production> P; set<Character> I, T, cE; int size; }; // cE: could Empty

const Character BUTTOM = -1;

String characterPow(Character x, int k) {
    String ret; ret.clear();
    for (int i = 0; i < k; i++) ret.push_back(x);
    return ret;
}

String conCat(String &a, String &b) {
    String ret = a;
    for (auto c: b) a.push_back(c);
    return ret;
}

String conCat(String &a, String &b, String &c) {
    return conCat(conCat(a, b), c);
}

String subString(String &a, int l, int r) { // [l, r] 
    String ret;
    if (l > r) return ret; // return an empty String
    for (int i = l; i <= r; i++) ret.push_back(a[i]);
    return ret;
}

String subRString(String &a, int l) { // [l, +oo)
    return subString(a, l, int(a.size())-1);
}

String validPrefix(Grammer &G, String &s, int k) {
    String ret;
    int cnt = 0;
    for (auto c: s) {
        if (cnt == k) break;
        if (!G.cE.count(c)) cnt++;
        ret += c;
    }
    return ret;
}

FollowSet extendFollowSet(Grammer &G, String s, bool needFirstEpsilon, int k) {
    s = validPrefix(G, s, k);
    queue<String> q; q.push(s);
    FollowSet fs; fs.insert(s);
    while (!q.empty()) {
        String ss = q.front(); q.pop();
        bool hasI = false; int fst = 0;
        for (; fst < ss.size(); ++fst) if (G.I.count(ss[fst])) { hasI = true; break; }
        if (!hasI) continue;
        for (auto &p: G.P) if(p.lhs == ss[fst]) {
            if (fst == 0 && p.rhs.size() == 0 && !needFirstEpsilon) continue;
            String ns = conCat(subString(ss, 0, fst-1), p.rhs, subRString(ss, fst+1));
            ns = validPrefix(G, ns, k);
            if (!fs.count(ns)) {
                fs.insert(ns);
                q.push(ns);
            }
        }
    }
    // find the string with k character in T
    FollowSet ret;
    for (auto &os: fs) {
        if (os.size() < k) continue;
        String ns = subString(os, 0, k-1);
        bool hasI = false;
        for (auto c: ns) if (G.I.count(c)) { hasI = true; break; }
        if (hasI) continue;
        if (!ret.count(ns)) ret.insert(ns);
    }
    return ret;
}

void extendStateSet(Grammer &G, StateSet &dst, StateSet &src, int k) {
    dst.clear();
    queue<State> q;
    for (auto st: src) { q.push(st); dst.insert(st); }
    while (!q.empty()) {
        State s = q.front(); q.pop();
        if (s.pos + 1 < G.P[s.id].rhs.size() && G.I.count(G.P[s.id].rhs[s.pos+1])) {
            FollowSet fs = extendFollowSet(G, conCat(subRString(G.P[s.id].rhs, s.pos+2), s.follow), true, k);
            for (size_t i = 0; i < G.P.size(); ++i) if (G.P[i].lhs == G.P[s.id].rhs[s.pos+1]) {
                for (auto &flw: fs) {
                    State cur{i, -1, flw};
                    if (!dst.count(cur)) {
                        dst.insert(cur);
                        q.push(cur);
                    }
                }
            }
        }
    }
}

void calcZ(Grammer &G, StateSet &Sp, FollowSet &Z, int k) {
    Z.clear();
    for (auto &s: Sp) if (s.pos + 1 < G.P[s.id].rhs.size()) {
        FollowSet fs = extendStateSet(G, conCat(subRString(G.P[s.id].rhs, s.pos+1), s.follow), false, k);
        for (auto &ns: fs) if (!Z.count(ns)) Z.insert(ns);
    }
}

void calcZp(Grammer &G, StateSet &Sp, FollowSet *Zp) {
    for (int i = 0; i < G.size; i++) Zp[i].clear();
    for (auto &s: Sp) if (s.pos + 1 == G.P[s.id].rhs.size()) {
        if (!Zp[s.id].count(s.follow)) Zp[s.id].insert(s.follow);
    }
}

bool isConflict(Grammer &G, FollowSet &Z, FollowSet *Zp) {
    int cnt = 0;
    for (auto &s: Z) for (int i = 0; i < G.size; ++i) if (Zp[i].count(s)) ++cnt;
    for (int i = 0; i < G.size; ++i) for (auto &s: Zp[i]) {
        if (Z.count(s)) ++cnt;
        for (int j = 0; j < G.size; ++j) if (j != i && Zp[j].count(s)) ++cnt;
    }
    if (cnt != 0) {
        fprintf(stderr, "There may be at least %d conflicts in this grammer.\n", cnt);
        return true;
    }
    return false;
}

StateSet nextStep(Grammer &G, StateSet &Sp, Character X) {
    StateSet ret;
    for (auto &s: Sp) if (s.pos + 1 < G.P[s.id].rhs.size()) {
        if (G.P[s.id].rhs[s.pos+1] == X) {
            State ns = s; ++ns.pos;
            ret.insert(ns); // no need to check duplication
        }
    }
    return ret;
}

// here, the G and s are both adjusted
ParseTree* Parse(Grammer G, String s, int k) {
    stack<Character>  CS; // Character Stack
    stack<StateSet>   SS; // State Set Stack
    stack<ParseTree*> TS; // Parse Root Stack
    FollowSet Z, *Zp = new FollowSet[G.size];
    StateSet S0; 
    S0.insert(State{0, -1, characterPow(BUTTOM, k)});
    SS.push(S0);
    size_t pt = 0; // pointer to Character in s
    while (pt + k < s.size()) {
        // STEP 1
        StateSet Sp; 
        extendStateSet(G, Sp, SS.top(), k);
        // STEP 2
        calcZ(G, Sp, Z, k);
        calcZp(G, Sp, Zp);
        if (isConflict(Z, Zp)) return NULL;
        bool isMatch = false;
        String sbs = subString(s, pt+1, pt+k);
        if (Z.count(sbs)) {
            ++pt; CS.push(s[pt]);
            TS.push(new ParseTree); TS.top().X = s[pt]; // new node on parse tree
            isMatch = true;
        } else {
            for (int i = 0; i < G.size; ++i) if (Zp[i].count(sbs)) {
                isMatch = true;
                State ns = *(Zp[i].find(sbs));
                int cnt = G.P[ns.id].rhs.size();
                vector<ParseTree*> tmpPtr;
                while (cnt--) { 
                    tmpPtr.push_back(TS.top());
                    CS.pop(); SS.pop(); TS.pop();
                }
                reverse(tmpPtr.begin(), tmpPtr.end());
                TS.push(new ParseTree(G.P[ns.id].lhs, tmpPtr));
                CS.push(G.P[ns.id].lhs);
                break;
            }
        }
        // STEP3
        if (!isMatch) {
            fprintf(stderr, "Wrong Syntax at position: %d\n", pt);
            return NULL;
        }
        extendStateSet(G, Sp, SS.top(), k);
        SS.push(nextStep(G, Sp, CS.top()));
    }
    delete[] Zp; // free the memory

    // return
    StateSet finS{0, 1, characterPow(BUTTOM, k)};
    if (SS.top().size == 1 && SS.top.count(finS)) { // accept
        return TS.top();
    } 
    fprintf(stderr, "Could not read the end of program\n");
    return NULL;
} 
