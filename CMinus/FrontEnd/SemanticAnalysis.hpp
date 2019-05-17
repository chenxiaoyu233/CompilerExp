#ifndef __SEMANTIC_ANALYSIS_H__
#define __SEMANTIC_ANALYSIS_H__
#include "FrontEnd.hpp"
#include "GrammerDefinition.hpp"

MCodeBase* SemanticAnalysis(LR::ParseTree *rt, vector<MCodeBase*> &semantic, vector<LexicalAnalyzer::LexicalItemInfo> &lexResult, int &cnt);

#endif
