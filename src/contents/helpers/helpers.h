#ifndef CONTENTS_HELPER_HELPERS_H
#define CONTENTS_HELPER_HELPERS_H

#include <cppcms/view.h>
#include "contents/Config.h"

namespace contents {

struct Helpers : public cppcms::base_content {
    std::string baseUrl;
    std::string lang;
};

}

#endif


