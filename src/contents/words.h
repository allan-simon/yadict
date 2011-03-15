#ifndef CONTENTS_WORDS_H
#define CONTENTS_WORDS_H

#include "contents/content.h"
#include "contents/forms/add_word.h"
#include "contents/forms/edit_word.h"
#include "contents/helpers/words.h"
#include "contents/forms/lang_filter.h"

namespace contents {

struct Words : public BaseContent {
    WordsHelper whc;
    int paginationSize;
};

struct WordsShow : public Words {
    std::string wordStr;
    std::string wordLang;
};



struct WordsAllIn : public Words {
    std::string filterLang;
};


struct WordsAdd : public BaseContent {
    forms::AddWord addWord; 
};


struct WordsEdit : public Words {
    forms::EditWord editWord; 
    std::string wordStr;
};



}

#endif
