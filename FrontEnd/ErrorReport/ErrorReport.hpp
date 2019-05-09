#ifndef __ERROR_REPORT_H__
#define __ERROR_REPORT_H__

#include "common.hpp"

class ErrorReport {
public:
    string context;
    ErrorReport(string context);
    void ReportAtPointInLine(size_t pos);
};

#endif
