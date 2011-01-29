#ifndef SHDICT_EDIT_META_FORM_H
#define SHDICT_EDIT_META_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct EditMeta : public cppcms::form {
        widgets::text value;
        widgets::hidden key;
        widgets::hidden wordId;
        widgets::submit submit;        
    
        // init of the form
        EditMeta() {
            *this + key + value + wordId + submit;

            key.name("key");
            value.name("value");
            
            wordId.name("wordId");
            wordId.value("0");


            submit.name("Edit meta");
            submit.value("edit");

            wordId.non_empty();
            key.non_empty();
            value.non_empty();

        };

    };
}



#endif

