#include "ErrorReport.hpp"

ErrorReport::ErrorReport(string context):context(context) { }

void ErrorReport::Report(string type, string content, size_t pos, size_t endpos) {
    int lineId = 1;
    if (endpos == -1) endpos = 1;
    else endpos -= pos;
    string curLine = "";
    for (auto c: context) {
        curLine += c;
        if (c == '\n') {
            if (pos < curLine.length()) {
                fprintf(stderr, "%s @ (line %d, col %zu) : %s\n",
                        ColorSelector(type, type).c_str(),
                        lineId, pos+1,
                        content.c_str()
                );
                fprintf(stderr, "%s", curLine.c_str());
                for (int i = 0; i < pos; ++i) fprintf(stderr, " ");
                string underLine = "";
                for (int i = 0; i < endpos; ++i) underLine += "^";
                underLine = Green(underLine);
                fprintf(stderr, "%s\n", underLine.c_str());
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

string ErrorReport::ColorSelector(string type, string content) {
    if (type == "error") return Red(content);
    if (type == "warning") return Yellow(content);
    return content;
}

string ErrorReport::Red(string content) {
#ifndef _WIN32
    return "\033[31m" + content + "\033[0m";
#else
    return content;
#endif
}

string ErrorReport::Green(string content) {
#ifndef _WIN32
    return "\033[32m" + content + "\033[0m";
#else
    return content;
#endif
}

string ErrorReport::Yellow(string content) {
#ifndef _WIN32
    return "\033[33m" + content + "\033[0m";
#else
    return content;
#endif
}
