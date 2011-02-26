#ifndef CONTENTS_MEANINGS_H
#define CONTENTS_MEANINGS_H 

#include "contents/content.h"
#include "contents/helpers/words.h"
#include "models/Meanings.h"
#include "contents/forms/add_meaning.h"
#include "contents/forms/edit_def_in_meaning.h"
#include "contents/forms/add_def_in_meaning.h"


namespace contents {

struct MeaningsAdd : public BaseContent {
    WordsHelper whc;
    forms::AddMeaning addMeaning; 
};



struct MeaningsAddDefIn : public BaseContent {
    WordsHelper whc;
    forms::AddDefInMeaning addDefInMeaning; 
    results::Meaning meaning;
    std::string defLang;
};



struct MeaningsEditDefIn : public BaseContent {
    WordsHelper whc;
    forms::EditDefInMeaning editDefInMeaning; 
    results::Meaning meaning;
    std::string defLang;
    int wordId;
};


struct MeaningsShow : public BaseContent {
    WordsHelper whc;
    results::Meaning meaning;
    forms::AddDefInMeaning addDefInMeaning; 
    int wordId;
};



}

#endif


