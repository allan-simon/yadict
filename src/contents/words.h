#ifndef CONTENTS_WORDS_H
#define CONTENTS_WORDS_H

#include "contents/content.h"
#include "contents/forms/add_word.h"
#include "contents/forms/edit_word.h"
#include "contents/helpers/words.h"

namespace contents {

struct Words : public BaseContent {
    WordsHelper whc;
};

struct WordsAllIn : public Words {
    WordsHelper whc;
    std::string filterLang;
};



struct WordsAdd : public BaseContent {
    forms::AddWord addWord; 
};


struct WordsEdit : public Words {
    forms::EditWord editWord; 
};




}

#endif
