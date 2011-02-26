#ifndef SHDICT_ADD_DEF_IN__MEANINGS_FORM_H
#define SHDICT_ADD_DEF_IN__MEANINGS_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct AddDefInMeaning : public cppcms::form {
        widgets::hidden wordId;
        // use to store the id of meaing we're going to add
        // a translation of the definition
        widgets::hidden meaningId;
        // store a description of the meaning
        widgets::text defText;
        // store in which language the description above is written in
        widgets::select defLang;
        widgets::submit submit;        
    
        // init of the form
        AddDefInMeaning() {
            *this + wordId + meaningId + defText + defLang + submit;

            wordId.name("wordId");
            wordId.value("0");

            meaningId.name("meaningId");
            meaningId.value("0");

            defText.name("text");

            ISOToNameMap isoToName = Languages::get_instance()->get_iso_to_name_map();
            ISOToNameMap::iterator itr;
            for(itr = isoToName.begin(); itr != isoToName.end(); ++itr){
                defLang.add(itr->second, itr->first);
            }

            submit.name("Add definition");
            submit.value("Add definition");

            meaningId.non_empty();
            defText.non_empty();
            defLang.non_empty();
        };

    };
}



#endif


