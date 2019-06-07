#ifndef __LR_K_H__
#define __LR_K_H__

#include "common.hpp"

namespace LR {
    typedef int Character;
    typedef vector<Character> String;
    struct StrCmp {
        bool operator () (const String &a, const String &b) const {
            if (a.size() != b.size()) return a.size() < b.size();
            for (size_t i = 0, r = min(a.size(), b.size()); i < r; ++i) {
                if (a[i] != b[i]) return a[i] < b[i];
            }
            return false;
        }
    };
    typedef set<String, StrCmp> FollowSet;
    struct ParseTree { 
        int pid; // production id
        Character X; vector<ParseTree*> child; 
        ParseTree(Character X):X(X) { child.clear(); pid = -1;} // pid == -1 表示叶子节点
        ParseTree(int pid, Character X, vector<ParseTree*> child): pid(pid), X(X), child(child) { }
    };
    struct State { 
        int id, pos; String follow; 
        State(int id, int pos, String follow):id(id), pos(pos), follow(follow) { }
    };
    struct StateCmp {
        bool operator () (const State &a, const State &b) const {
            StrCmp sp;
            if (a.id == b.id) {
                if (a.pos == b.pos) {
                    return sp(a.follow, b.follow);
                }
                return a.pos < b.pos;
            }
            return a.id < b.id;
        }
    };
    typedef set<State, StateCmp> StateSet;
    struct StateSetCmp {
        bool operator () (const StateSet &a, const StateSet &b) const {
            StateCmp sp;
            if (a.size() != b.size()) {
                return a.size() < b.size();
            }
            vector<State> veca, vecb;
            veca.clear(); vecb.clear();
            for (auto s: a) veca.push_back(s); sort(veca.begin(), veca.end(), sp);
            for (auto s: b) vecb.push_back(s); sort(vecb.begin(), vecb.end(), sp);
            for (size_t i = 0; i < a.size(); ++i) {
                if (sp(veca[i], vecb[i]) || sp(vecb[i], veca[i])) {
                    return sp(veca[i], vecb[i]);
                }
            }
            return false;
        }
    };
    struct Production { Character lhs; String rhs; };
    struct Grammer { vector<Production> P; set<Character> I, T, cE; }; // cE: could Empty

    const Character BUTTOM = -1;

    /* Main Functions */
    ParseTree* Parse(Grammer G, String s, int k, int &errorAt);
    /* LR Table generator */
    void GenerateLRTable(Grammer G, int k, string node_dump, string trans_dump);
    ParseTree* ParseWithLRTable(Grammer G, String s, int k, int &errorAt, string node_dump, string trans_dump);

    /* Debug Logger */
    void ParseTreeLog(ParseTree *rt, vector<string> &content, vector<string> *lexResult);
}

#endif
