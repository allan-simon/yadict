#ifndef CONTENTS_WORDS_H
#define CONTENTS_WORDS_H

#include "contents/content.h"
#include "contents/forms/add_word.h"

extern "C" {
#include <tato/fetcher.h>
#include <tato/hyperitem.h>
}

namespace contents {

struct Words : public BaseContent {
    TatoHyperItemFetcher *fetcher;
};


struct WordsAdd : public BaseContent {
    forms::AddWord addWord; 
};




}

#endif
