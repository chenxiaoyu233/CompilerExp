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

LexicalAnalyzer::LexicalItemInfo(string content, string symbolType, size_t begin, size_t end):
	content(content), symbolType(symbolType), begin(begin), end(end) { }

LexicalAnalyzer::LexicalErrorInfo(LexicalErrorType errorType, size_t position):
	errorType(errorType), position(position) { }

LexicalAnalyzer::LexicalSymbolInfo(string regExp, string symbolType, bool isReturn):
	regExp(regExp), symbolType(symbolType), isReturn(isReturn) { }

void LexicalAnalyzer::AddLexicalItem(string regExp, string symbolType, bool isReturn) {

	// store the infomation
	LexicalSymbolInfo* info = new LexicalSymbolInfo(regExp, symbolType, isReturn);
	symbolTypeList.push_back(info);

	// construct the DFA
	FA fa =  RegExp(regExp).DFA();
	for (auto st: fa.accept) {
		st -> SetInfo(info);
	}

	// merge into dfa
	dfa = dfa | fa;
}

LexicalErrorInfo LexicalAnalyzer::LexicalAnalyze(string context, bool isClean) {
	this -> context = context;
	if(isClean) itemList.clear();

	State* spt = dfa.start;
	State* lspt = NULL;
	for (size_t lpt = 0, rpt = 0, ipt = 0; ipt < context.length(); ipt++) {
		if (spt -> type == StateType::accept) {
			lspt = spt;
			rpt = ipt;
		}
		spt = dfa.Next(spt);
		if (spt == NULL) {
			if (lspt != NULL) {
				string content = context.substr(lpt, rpt-lpt);
				string symbolType = lspt -> GetInfo() -> symbolType;
				if (lspt -> GetInfo() -> isReturn)
					itemList.push_back(LexicalItemInfo(content, symbolType, lpt, rpt));
			} else {
				return LexicalErrorInfo(LexicalErrorInfo::LexicalErrorType::MisMatch, ipt);
			}
		}
	}
	return LexicalErrorInfo(LexicalErrorInfo::LexicalErrorType::NoError);
}

vector<LexicalItemInfo> LexicalAnalyzer::Result() {
	return itemList;
}
