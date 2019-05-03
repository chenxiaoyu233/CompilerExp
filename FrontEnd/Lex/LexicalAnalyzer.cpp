#include "LexicalAnalyzer.hpp"

LexicalAnalyzer::LexicalAnalyzer():dfa(FA::EmptySet()) {
	itemList.clear();
	context.clear();
	symbolTypeList.clear();
}

LexicalAnalyzer::~LexicalAnalyzer() {
	for (auto spt: symbolTypeList) {
		delete spt;
	}
}

LexicalAnalyzer::LexicalItemInfo::LexicalItemInfo(string content, string symbolType, size_t begin, size_t end):
	content(content), symbolType(symbolType), begin(begin), end(end) { }

LexicalAnalyzer::LexicalErrorInfo::LexicalErrorInfo(LexicalErrorType errorType, size_t position):
	errorType(errorType), position(position) { }

LexicalAnalyzer::LexicalSymbolInfo::LexicalSymbolInfo(string regExp, string symbolType, bool isReturn, size_t level):
	regExp(regExp), symbolType(symbolType), isReturn(isReturn), level(level) { }

void LexicalAnalyzer::AddLexicalItem(string regExp, string symbolType, bool isReturn, size_t level) {

	// store the infomation
	LexicalSymbolInfo* info = new LexicalSymbolInfo(regExp, symbolType, isReturn, level);
	symbolTypeList.push_back(info);

	// construct the DFA
	FA fa =  RegExp(regExp).DFA();
	for (auto st: fa.accept) {
		st -> InfoPushBack(info);
	}

	// merge into dfa
	dfa = dfa | fa;
}

LexicalAnalyzer::LexicalErrorInfo LexicalAnalyzer::LexicalAnalyze(string context, bool isClean) {
	if (dfa.type == FAType::NFA) {
		dfa = dfa.NtoD();
	}

	this -> context = context;
	if(isClean) itemList.clear();

	State* spt = dfa.start;
	State* lspt = NULL;
	for (size_t lpt = 0, rpt = 0, ipt = 0; ipt < context.length(); ) {
		if (spt -> type == StateType::accept) {
			lspt = spt;
			rpt = ipt;
		}
		spt = dfa.Next(spt, string() + context[ipt]);
		if (spt == NULL) {
			if (lspt != NULL) {
				string content = context.substr(lpt, rpt-lpt);
				vector<void*> info = lspt -> GetInfo();
				// get the maximal symbol level
				size_t maxLevel = 0;
				for (auto item: info) {
					LexicalSymbolInfo* infoPt = (LexicalSymbolInfo*) item;
					maxLevel = max(maxLevel, infoPt -> level);
				}
				// collect the symbolType
				for (auto item: info) {
					LexicalSymbolInfo* infoPt = (LexicalSymbolInfo*) item;
					if (infoPt -> level != maxLevel) continue;
					string symbolType = infoPt -> symbolType;
					if (infoPt -> isReturn)
						itemList.push_back(LexicalItemInfo(content, symbolType, lpt, rpt));
				}
				// reset the DFA
                ipt = lpt = rpt;
                lspt = NULL;
                spt = dfa.start;
			} else {
				return LexicalErrorInfo(LexicalErrorInfo::LexicalErrorType::MisMatch, ipt);
			}
        } else ipt++;
	}
	return LexicalErrorInfo(LexicalErrorInfo::LexicalErrorType::NoError);
}

vector<LexicalAnalyzer::LexicalItemInfo> LexicalAnalyzer::Result() {
	return itemList;
}
