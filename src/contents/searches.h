#ifndef CONTENTS_SEARCHES_H
#define CONTENTS_SEARCHES_H

#include "contents/content.h"
#include "contents/forms/search.h"
#include "contents/helpers/words.h"

namespace contents {

struct SearchesSimple : public BaseContent {
    WordsHelper whc;
    std::string queryStr;
    std::string queryLang;
};

struct SearchesShowResult : public SearchesSimple {
    int paginationSize;
};



};

#endif
