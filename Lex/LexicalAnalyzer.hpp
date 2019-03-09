#ifndef __LEXICAL_ANALYZER_H__
#define __LEXICAL_ANALYZER_H__

#include "common.hpp"

class LexicalAnalyzer {
private:
	struct LexicalItemInfo {
		// you could add something here
		string content;
		string symbolType;
	};
	struct LexicalSymbolInfo {
		string regExp;
		string symbolType;
		bool isReturn;
	};
	struct LexicalErrorInfo {
		enum LexicalErrorType {
			NoError, // no error here
			MisMatch // do not match any regular expression
		};
		LexicalErrorType errorType;
		size_t position;
	};
	FA dfa;
	vector<LexicalItemInfo> itemList;
	string contex;
	vector<LexicalSymbolInfo*> symbolTypeList;
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	// use the regular expression to define a lexical item.
	// @param string regExp: The regular expression to recognize the lexical item.
	// @param string symbolType: The symbol to represent the lexical item.
	// @param bool isReturn: report or not when find this kind of lexical item in the stream.
	// P.S. all the regular expressions should be disjoint.
	void AddLexicalItem(string regExp, string symbolType, bool isReturn = true);

	// for a given context, preform the lexical analysis on it.
	// @param string context: the context for lexical analysis
	// @return Lexicalerrorinfo: the error infomation for the lexical analysis.
	LexicalErrorInfo LexicalAnalyze(string context, bool isClean);

	// get the result for lexical analysis.
	// @return vector<LexicalItemInfo>: return the lexical items in a vector.
	vector<LexicalItemInfo> Result();
};

#endif
