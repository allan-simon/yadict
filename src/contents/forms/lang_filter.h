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

            //TODO factorize it is also share with several other forms (such as search)
            ISOToNameMap isoToName = Languages::get_instance()->get_iso_to_name_map();
            ISOToNameMap::iterator itr;
            for(itr = isoToName.begin(); itr != isoToName.end(); ++itr){
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

