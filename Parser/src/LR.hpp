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
        Character X; vector<ParseTree*> child; 
        ParseTree(Character X):X(X) { child.clear(); }
        ParseTree(Character X, vector<ParseTree*> child):X(X), child(child) { }
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
    struct Production { Character lhs; String rhs; };
    struct Grammer { vector<Production> P; set<Character> I, T, cE; }; // cE: could Empty

    const Character BUTTOM = -1;

    /* Main Functions */
    ParseTree* Parse(Grammer G, String s, int k);

    /* Debug Logger */
    void ParseTreeLog(ParseTree *rt, char **content);
}

#endif
