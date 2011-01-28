#ifndef SHDICT_ADD_META_FORM_H
#define SHDICT_ADD_META_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct AddMeta : public cppcms::form {
        widgets::text key;
        widgets::text value;
        widgets::hidden wordId;
        widgets::submit submit;        
    
        // init of the form
        AddMeta() {
            *this + key + value + wordId + submit;

            key.name("key");
            value.name("value");
            
            wordId.name("wordId");
            wordId.value("0");


            submit.name("Add meta");
            submit.value("add");

            wordId.non_empty();
            key.non_empty();
            value.non_empty();

        };

    };
}



#endif

