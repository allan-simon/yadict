#ifndef SHDICT_LANG_FILTER_FORM_H
#define SHDICT_LANG_FILTER_FORM_H

#include <cppcms/form.h>
#include "generics/Languages.h"

namespace forms {

	struct LangsFilter : public cppcms::form {
        cppcms::widgets::select langFilter;
		cppcms::widgets::submit submit;
		
		LangsFilter() {
            *this +  langFilter + submit;

            LangMap langMap = Languages::get_instance()->get_map();
            LangMap::iterator itr;
            for(itr = langMap.begin(); itr != langMap.end(); ++itr){
                langFilter.add(itr->second,itr->first);
            }

            submit.name("Filter");
            submit.value("filter");
            // TODO for select I'm sure there's something more specific for
            // selects to say "value must one the possible values of the select"
            langFilter.non_empty();
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

