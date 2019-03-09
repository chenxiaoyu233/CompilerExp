#include "LexicalAnalyzer.hpp"

LexicalAnalyzer::LexicalAnalyzer():dfa(FA(FAType::NFA)) {
	itemList.clear();
	contex.clear();
	symbolTypeList.clear();
}

LexicalAnalyzer::~LexicalAnalyzer() {
	for (auto spt: symbolTypeList) {
		delete spt;
	}
}

void LexicalAnalyzer::AddLexicalItem(string regExp, string symbolType, bool isReturn) {
	LexicalSymbolInfo* info = new LexicalSymbolInfo();

	// store the infomation
	info -> regExp = regExp;
	info -> symbolType = symbolType;
	info -> isReturn = isReturn;
	symbolTypeList.push_back(info);

	// construct the DFA
	FA fa =  RegExp(regExp).DFA();
	for (auto st: fa.accept) {
		st -> SetInfo(info);
	}

	// merge into dfa
	dfa = dfa | fa;
}

LexicalErrorInfo LexicalAnalyzer::LexicalAnalyze(int context, bool isClean) {
}
