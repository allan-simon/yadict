#ifndef CONTENTS_TRANSLATIONS_H
#define CONTENTS_TRANSLATIONS_H 

#include "contents/content.h"
#include "contents/helpers/words.h"
#include "contents/forms/add_translation.h"
#include "contents/forms/add_trans_to_meaning.h"

namespace contents {

struct TranslationsAdd : public BaseContent {
    WordsHelper whc;
    forms::AddTranslation addTranslation; 
};


struct TranslationsAddToMeaning : public BaseContent {
    WordsHelper whc;
    forms::AddTransToMeaning addTransToMeaning; 
    int meaningId;
};



}

#endif

