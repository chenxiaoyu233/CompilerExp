#include "FrontEnd.hpp"

void BackEndImplement::lexErrorHandler(LexicalAnalyzer::LexicalErrorInfo errInfo){
    if (errInfo.errorType == LexicalAnalyzer::LexicalErrorInfo::LexicalErrorType::MisMatch) {
        //fprintf(stderr, "Lex: there is something wrong at pos %zu\n", errInfo.position);
        //fprintf(stderr, "context: %s\n", context.c_str());
        //fprintf(stderr, "context length: %zu\n", context.length());
        ErrorReport(context).Report("error", "Lex Error", errInfo.position);
        //ErrorReport(context).ReportAtPointInLine(250);
        exit(233);
    }
}
