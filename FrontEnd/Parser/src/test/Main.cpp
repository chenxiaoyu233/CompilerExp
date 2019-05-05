#include "LR.hpp"
using namespace LR;

vector<string> content;

void test1() {
    Grammer G {
        { {0, {1, BUTTOM}},     // S -> E-|
          {1, {4, 2}},          // E -> -T
          {1, {2}},             // E -> T
          {1, {1, 4, 2}},       // E -> E - T
          {2, {3}},             // T -> P
          {2, {2, 5, 3}},       // T -> T * P
          {3, {6}},             // P -> a
          {3, {7, 1, 8}} },     // P -> (E)
        { 0, 1, 2, 3 },         // {S, E, T, P}
        { -1, 4, 5, 6, 7, 8 },  // {-|, -, *, a, (, )}
        { }
    };
    String s{
        6, 4, 7, 4, 6, 5, 6, 4, 6, 8, BUTTOM
//      a  -  (  -  a  *  a  -  a  )  -|
    };
    content.push_back("S");
    content.push_back("E");
    content.push_back("T");
    content.push_back("P");
    content.push_back("-");
    content.push_back("*");
    content.push_back("a");
    content.push_back("(");
    content.push_back(")");
    ParseTree *rt = Parse(G, s, 1);
    ParseTreeLog(rt, content);
}

void test2() {
    Grammer G {
        { {0, {1, BUTTOM}},     // S -> E-|
            {1, {4, 2}},          // E -> -T
            {1, {2}},             // E -> T
            {1, {1, 4, 2}},       // E -> E - T
            {1, {1, 9, 2}},       // E -> E + T
            {2, {3}},             // T -> P
            {2, {2, 5, 3}},       // T -> T * P
            {2, {2, 10, 3}},      // T -> T / P
            {3, {6}},             // P -> a
            {3, {7, 1, 8}} },     // P -> (E)
        { 0, 1, 2, 3 },         // {S, E, T, P}
        { -1, 4, 5, 6, 7, 8, 9, 10 },  // {-|, -, *, a, (, ), +, /}
        { }
    };
    String s{
        7, 6, 9, 7, 7, 6, 4, 6, 8, 5, 7, 6, 10, 6, 8, 8, 8, BUTTOM
//      (  a  +  (  (  a  -  a  )  *  (  a  /   a  )  )  ), -|
    };
    content.push_back("S");
    content.push_back("E");
    content.push_back("T");
    content.push_back("P");
    content.push_back("-");
    content.push_back("*");
    content.push_back("a");
    content.push_back("(");
    content.push_back(")");
    content.push_back("+");
    content.push_back("/");
    ParseTree *rt = Parse(G, s, 1);
    ParseTreeLog(rt, content);
}

void test3() {
    Grammer G {
        {   {0, {1, BUTTOM}},       // S' -> S-|
            {1, {}},                // S -> 
            {1, {4, 2, 5, 1}},      // S -> aAbS
            {1, {5, 3, 4, 1}},      // S -> bBaS
            {2, {}},                // A -> 
            {2, {4, 2, 5, 2}},      // A -> aAbA
            {3, {}},                // B ->
            {3, {5, 3, 4, 3}} },    // B -> bBaB
        { 0, 1, 2, 3 },             // {S', S, A, B}
        { 4, 5 },                   // {a, b}
        { 1, 2, 3 }                 // {S, A, B} 
    };
    String s{
        4, 4, 4, 4, 5, 5, 5, 5, BUTTOM
//      a, a, a, a, b, b, b, b, -|
    };
    content.push_back("S\'");
    content.push_back("S");
    content.push_back("A");
    content.push_back("B");
    content.push_back("a");
    content.push_back("b");
    ParseTree *rt = Parse(G, s, 1);
    ParseTreeLog(rt, content);
}

void test4() {
    Grammer G {
        { {0, {1, BUTTOM}},     // S -> E-|
            {1, {4, 2}},          // E -> -T
            {1, {2}},             // E -> T
            {1, {1, 4, 2}},       // E -> E - T
            {1, {1, 9, 2}},       // E -> E + T
            {2, {3}},             // T -> P
            {2, {2, 5, 3}},       // T -> T * P
            {2, {2, 10, 3}},      // T -> T / P
            {3, {6}},             // P -> a
            {3, {7, 1, 8}} },     // P -> (E)
        { 0, 1, 2, 3 },         // {S, E, T, P}
        { -1, 4, 5, 6, 7, 8, 9, 10 },  // {-|, -, *, a, (, ), +, /}
        { }
    };
    String s{
        6, 9, 6, 4, 6, 5, 6, 10, 6, BUTTOM
//      a  +  a  -  a  *  a  /   a  -|
    };
    content.push_back("S");
    content.push_back("E");
    content.push_back("T");
    content.push_back("P");
    content.push_back("-");
    content.push_back("*");
    content.push_back("a");
    content.push_back("(");
    content.push_back(")");
    content.push_back("+");
    content.push_back("/");
    ParseTree *rt = Parse(G, s, 1);
    ParseTreeLog(rt, content);
}

// use for test
int main() {
    test4();
    return 0;
}
