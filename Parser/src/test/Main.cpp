#include "LR.hpp"
using namespace LR;

char **content;

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
    content = new char*[15];
    for (int i = 0; i < 15; i++) content[i] = new char [3];
    content[0][0] = 'S'; content[0][1] = '\0';
    content[1][0] = 'E'; content[1][1] = '\0';
    content[2][0] = 'T'; content[2][1] = '\0';
    content[3][0] = 'P'; content[3][1] = '\0';
    content[4][0] = '-'; content[4][1] = '\0';
    content[5][0] = '*'; content[5][1] = '\0';
    content[6][0] = 'a'; content[6][1] = '\0';
    content[7][0] = '('; content[7][1] = '\0';
    content[8][0] = ')'; content[8][1] = '\0';
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
    content = new char*[15];
    for (int i = 0; i < 15; i++) content[i] = new char [3];
    content[0][0] = 'S'; content[0][1] = '\0';
    content[1][0] = 'E'; content[1][1] = '\0';
    content[2][0] = 'T'; content[2][1] = '\0';
    content[3][0] = 'P'; content[3][1] = '\0';
    content[4][0] = '-'; content[4][1] = '\0';
    content[5][0] = '*'; content[5][1] = '\0';
    content[6][0] = 'a'; content[6][1] = '\0';
    content[7][0] = '('; content[7][1] = '\0';
    content[8][0] = ')'; content[8][1] = '\0';
    content[9][0] = '+'; content[8][1] = '\0';
    content[10][0] = '/'; content[8][1] = '\0';
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
    content = new char*[15];
    for (int i = 0; i < 15; i++) content[i] = new char [3];
    content[0][0] = 'S'; content[0][1] = '\''; content[0][2] = '\0';
    content[1][0] = 'S'; content[1][1] = '\0';
    content[2][0] = 'A'; content[1][1] = '\0';
    content[3][0] = 'B'; content[1][1] = '\0';
    content[4][0] = 'a'; content[1][1] = '\0';
    content[5][0] = 'b'; content[1][1] = '\0';
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
    content = new char*[15];
    for (int i = 0; i < 15; i++) content[i] = new char [3];
    content[0][0] = 'S'; content[0][1] = '\0';
    content[1][0] = 'E'; content[1][1] = '\0';
    content[2][0] = 'T'; content[2][1] = '\0';
    content[3][0] = 'P'; content[3][1] = '\0';
    content[4][0] = '-'; content[4][1] = '\0';
    content[5][0] = '*'; content[5][1] = '\0';
    content[6][0] = 'a'; content[6][1] = '\0';
    content[7][0] = '('; content[7][1] = '\0';
    content[8][0] = ')'; content[8][1] = '\0';
    content[9][0] = '+'; content[8][1] = '\0';
    content[10][0] = '/'; content[8][1] = '\0';
    ParseTree *rt = Parse(G, s, 1);
    ParseTreeLog(rt, content);
}

// use for test
int main() {
    test4();
    return 0;
}
