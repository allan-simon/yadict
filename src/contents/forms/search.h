#ifndef SHDICT_SEARCH_FORM_H
#define SHDICT_SEARCH_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

namespace forms {

	struct SearchesSimple : public cppcms::form {
		cppcms::widgets::text query;
        cppcms::widgets::select wordLang;
		cppcms::widgets::submit submit;
		
		SearchesSimple() {
            *this + query + wordLang + submit;

            query.name("query");

            ISOToNameMap isoToName = Languages::get_instance()->get_iso_to_name_map();
            ISOToNameMap::iterator itr;
            for(itr = isoToName.begin(); itr != isoToName.end(); ++itr){
                wordLang.add(itr->second,itr->first);
            }

            submit.name("Add word");
            submit.value("add");

            query.non_empty();
            // TODO for select I'm sure there's something more specific for
            // selects to say "value must one the possible values of the select"
            wordLang.non_empty();
        };

		virtual bool validate() {
			if ( ! form::validate()) {
				return false;
			}
			return true;
		}
		
	};

}
#endif

