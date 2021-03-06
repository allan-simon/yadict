#ifndef SHDICT_ADD_WORD_FORM_H
#define SHDICT_ADD_WORD_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct AddWord : public cppcms::form {
        widgets::text wordString;
        widgets::select wordLang;
        widgets::submit submit;        
    
        // init of the form
        AddWord() {
            *this + wordString + wordLang + submit;

            wordString.name("str");

            ISOToNameMap isoToName = Languages::get_instance()->get_iso_to_name_map();
            ISOToNameMap::iterator itr;
            for(itr = isoToName.begin(); itr != isoToName.end(); ++itr){
                wordLang.add(itr->second,itr->first);
            }

            submit.name("Add word");
            submit.value("add");

            wordString.non_empty();
            // TODO for select I'm sure there's something more specific for
            // selects to say "value must one the possible values of the select"
            wordLang.non_empty();
        };

    };
}



#endif

