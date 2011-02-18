#ifndef SHDICT_ADD_TRANSLATE_FORM_H
#define SHDICT_ADD_TRANSLATE_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct AddTranslation : public cppcms::form {
        // use to store the id of word we're going to translate
        widgets::hidden origWordId;
        // use to store the id of the translation relation we're going
        // to add an element to
        // set to 0 if for the moment the word has no translation
        widgets::hidden translationId;
        widgets::text wordString;
        widgets::select wordLang;
        widgets::submit submit;        
    
        // init of the form
        AddTranslation() {
            *this + origWordId + translationId + wordString + wordLang + submit;

            translationId.name("transId");
            translationId.value("0");

            origWordId.name("origId");
            origWordId.value("0");

            wordString.name("str");


            ISOToNameMap isoToName = Languages::get_instance()->get_iso_to_name_map();
            ISOToNameMap::iterator itr;
            for(itr = isoToName.begin(); itr != isoToName.end(); ++itr){
                wordLang.add(itr->second, itr->first);
            }

            submit.name("Add word");
            submit.value("add");

            origWordId.non_empty();
            translationId.non_empty();
            wordString.non_empty();
            wordLang.non_empty();
        };

    };
}



#endif


