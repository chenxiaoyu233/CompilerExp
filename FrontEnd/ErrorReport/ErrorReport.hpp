#ifndef __ERROR_REPORT_H__
#define __ERROR_REPORT_H__

#include "common.hpp"

class ErrorReport {
public:
    string context;
    ErrorReport(string context);
    void Report(string type, string content, size_t pos, size_t endpos = -1);
    string Red(string content);
    string Green(string content);
    string Yellow(string content);
    string ColorSelector(string type, string content);
};

#endif
