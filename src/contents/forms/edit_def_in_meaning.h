#ifndef SHDICT_EDIT_DEF_IN_MEANINGS_FORM_H
#define SHDICT_EDIT_DEF_IN_MEANINGS_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

using namespace cppcms;

namespace forms {

    struct EditDefInMeaning : public cppcms::form {
        widgets::hidden defLang;
        widgets::hidden meaningId;
        // we need it for redirection
        widgets::hidden wordId;
        // store a description of the meaning
        widgets::text defText;
        widgets::submit submit;        
    
        // init of the form
        EditDefInMeaning() {
            *this + wordId + meaningId + defText + defLang + submit;


            wordId.name("wordId");
            wordId.value("0");

            meaningId.name("meaningId");
            meaningId.value("0");

            defText.name("text");
            defLang.name("lang");


            submit.name("Edit Meaning");
            submit.value("Edit meaning");

            meaningId.non_empty();
            defText.non_empty();
            defLang.non_empty();
        };

    };
};



#endif



