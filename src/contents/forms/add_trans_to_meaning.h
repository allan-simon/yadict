#ifndef SHDICT_ADD_TRANS_TO_MEANINGS_FORM_H
#define SHDICT_ADD_TRANS_TO_MEANINGS_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct AddTransToMeaning : public cppcms::form {
        // use to store the id of word we're going to translate
        widgets::hidden wordId;
        // use to store the id of the meaning to which the translation
        // will be associated
        widgets::hidden meaningId;
        
        // text and lang of the translation
        widgets::text transText;
        widgets::select transLang;

        widgets::submit submit;        
    
        // init of the form
        AddTransToMeaning() {
            *this + wordId + meaningId + transText + transLang + submit;

            wordId.name("wordId");
            wordId.value("0");

            meaningId.name("meaningId");
            meaningId.value("0");

            transText.name("text");


            LangMap langMap = Languages::get_instance()->get_map();
            LangMap::iterator itr;
            for(itr = langMap.begin(); itr != langMap.end(); ++itr){
                transLang.add(itr->second, itr->first);
            }

            submit.name("Add Translation");
            submit.value("Add Translation");

            wordId.non_empty();
            meaningId.non_empty();
            transText.non_empty();
            transLang.non_empty();
        };

    };
}



#endif



