#ifndef __LEXICAL_ANALYZER_H__
#define __LEXICAL_ANALYZER_H__

#include "common.hpp"

class LexicalAnalyzer {
public:
	struct LexicalItemInfo {
		// you could add something here
		string content;
		string symbolType;
		size_t begin, end; // [begin, end)
		LexicalItemInfo(string content, string symbolType, size_t begin, size_t end);
	};
	struct LexicalSymbolInfo {
		string regExp;
		string symbolType;
		bool isReturn;
		size_t level;
		LexicalSymbolInfo(string regExp, string symbolType, bool isReturn, size_t level);
	};
	struct LexicalErrorInfo {
		enum LexicalErrorType {
			NoError, // no error here
			MisMatch // do not match any regular expression
		};
		LexicalErrorType errorType;
		size_t position;
		LexicalErrorInfo(LexicalErrorType errorType, size_t position = -1);
	};
private:
	FA dfa;
	string context;
	vector<LexicalItemInfo> itemList;
	vector<LexicalSymbolInfo*> symbolTypeList;
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	// use the regular expression to define a lexical item.
	// @param string regExp: The regular expression to recognize the lexical item.
	// @param string symbolType: The symbol to represent the lexical item.
	// @param bool isReturn: report or not when find this kind of lexical item in the stream.
	// @param size_t level: the level of the symbolType. (higher level one will cover the lower ones).
	// P.S. all the regular expressions should be disjoint.
	void AddLexicalItem(string regExp, string symbolType, bool isReturn = true, size_t level = 0);

	// for a given context, preform the lexical analysis on it.
	// @param string context: the context for lexical analysis
	// @param bool isClean: clean or not clean the itemlist (used to store the answer).
	// @return Lexicalerrorinfo: the error infomation for the lexical analysis.
	LexicalAnalyzer::LexicalErrorInfo LexicalAnalyze(string context, bool isClean = true);

	// get the result for lexical analysis.
	// @return vector<LexicalItemInfo>: return the lexical items in a vector.
	vector<LexicalAnalyzer::LexicalItemInfo> Result();
};

#endif
