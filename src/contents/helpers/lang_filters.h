#ifndef CONTENTS_HELPER_LANG_FILTERS_H
#define CONTENTS_HELPER_LANG_FILTERS_H

#include "helpers.h"
#include "contents/forms/lang_filter.h"

namespace contents {

struct LangsFiltersHelper : public Helpers {
    std::string currentFilter;
    forms::LangsFilter langsFilter;
    std::string filterTreatUrl;
    
    public:
        LangsFiltersHelper(std::string _filterTreatUrl) {
            filterTreatUrl = _filterTreatUrl;
        }

};

}

#endif


