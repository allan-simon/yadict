#ifndef SHDICT_EDIT_WORD_FORM_H
#define SHDICT_EDIT_WORD_FORM_H

#include <cppcms/form.h>
#include "add_word.h"
using namespace cppcms;

namespace forms {

    struct EditWord : public AddWord{
        widgets::hidden wordId;
        // init of the form
        EditWord(): AddWord() {

            wordId.name("id");
            wordId.value("0");
            wordId.non_empty();
            // TODO add in the doc to not forget this line
            // when you create a child from a form:)
            *this + wordId;

            submit.value("edit");

        };

        

    };
}



#endif


