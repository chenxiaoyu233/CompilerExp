#include "ErrorReport.hpp"

ErrorReport::ErrorReport(string context):context(context) { }

void ErrorReport::ReportAtPointInLine(size_t pos) {
    int lineId = 1;
    string curLine = "";
    for (auto c: context) {
        if (c == '\n') {
            if (pos < curLine.length()) {
                fprintf(stderr, "line %d:\n", lineId);
                fprintf(stderr, "%s\n", curLine.c_str());
                for (int i = 1; i < pos; ++i) fprintf(stderr, " ");
                fprintf(stderr, "^\n");
                return;
            }
            pos -= curLine.length();
            curLine = "";
            ++lineId;
        } else {
            curLine += c;
        }
    }
}
