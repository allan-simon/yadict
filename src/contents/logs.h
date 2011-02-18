#ifndef CONTENTS_LOGS_H
#define CONTENTS_lOGS_H

#include "contents/content.h"
#include "models/Logs.h"

namespace contents {

    struct Logs : public BaseContent {
        models::WordsLogList wordsLogs;
    };

    struct LogsShowLastMetas : public BaseContent {
        models::MetasLogList metasLogs;
    };


};
#endif
