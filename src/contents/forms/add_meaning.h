#ifndef SHDICT_ADD_MEANINGS_FORM_H
#define SHDICT_ADD_MEANINGS_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct AddMeaning : public cppcms::form {
        // use to store the id of word we're going to translate
        widgets::hidden origWordId;
        // store a description of the meaning
        widgets::text definitionText;
        // store in which language the description above is written in
        widgets::select definitionLang;
        widgets::submit submit;        
    
        // init of the form
        AddMeaning() {
            *this + origWordId + definitionText + definitionLang + submit;

            origWordId.name("origId");
            origWordId.value("0");

            definitionText.name("text");


            LangMap langMap = Languages::get_instance()->get_map();
            LangMap::iterator itr;
            for(itr = langMap.begin(); itr != langMap.end(); ++itr){
                definitionLang.add(itr->second, itr->first);
            }

            submit.name("Add Meaning");
            submit.value("Add meaning");

            origWordId.non_empty();
            definitionText.non_empty();
            definitionLang.non_empty();
        };

    };
}



#endif


