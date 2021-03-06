#include "FrontEnd.hpp"

void FrontEndImplement::lexErrorHandler(LexicalAnalyzer::LexicalErrorInfo errInfo){
    if (errInfo.errorType == LexicalAnalyzer::LexicalErrorInfo::LexicalErrorType::MisMatch) {
        //fprintf(stderr, "there is something wrong at pos %zu\n", errInfo.position);
        ErrorReport(context).Report("error", "Lex Error", errInfo.position);
        exit(233);
    }
}
