#ifndef CONTENTS_TRANSLATIONS_H
#define CONTENTS_TRANSLATIONS_H 

#include "contents/content.h"
#include "contents/helpers/words.h"
#include "contents/forms/add_translation.h"

namespace contents {

struct TranslationsAdd : public BaseContent {
    WordsHelper whc;
    forms::AddTranslation addTranslation; 
};



}

#endif

