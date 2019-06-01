#include "ErrorReport.hpp"

ErrorReport::ErrorReport(string context):context(context) { }

void ErrorReport::ReportAtPointInLine(size_t pos, size_t endpos) {
    int lineId = 1;
    if (endpos == -1) endpos = 1;
    else endpos -= pos;
    string curLine = "";
    for (auto c: context) {
        curLine += c;
        if (c == '\n') {
            if (pos < curLine.length()) {
                fprintf(stderr, "line %d : col %zu\n", lineId, pos+1);
                fprintf(stderr, "%s", curLine.c_str());
                for (int i = 0; i < pos; ++i) fprintf(stderr, " ");
                for (int i = 0; i < endpos; ++i) fprintf(stderr, "^");
                fprintf(stderr, "\n");
                return;
            }
            pos -= curLine.length();
            curLine = "";
            ++lineId;
        }
    }
    fprintf(stderr, "lineId: %d\n", lineId);
    fprintf(stderr, "curLine: %s\n", curLine.c_str());
}
