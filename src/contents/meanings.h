#ifndef CONTENTS_MEANINGS_H
#define CONTENTS_MEANINGS_H 

#include "contents/content.h"
#include "contents/helpers/words.h"
#include "contents/forms/add_meaning.h"
#include "contents/forms/edit_meaning.h"


namespace contents {

struct MeaningsAdd : public BaseContent {
    WordsHelper whc;
    forms::AddMeaning addMeaning; 
};

struct MeaningsEdit : public BaseContent {
    WordsHelper whc;
    forms::EditMeaning editMeaning; 
};




}

#endif


