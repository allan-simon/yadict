#ifndef CONTENTS_BASE_CONTENT_H
#define CONTENTS_BASE_CONTENT_H

#include <cppcms/view.h>
#include "forms/search.h"
#include "contents/Config.h"
#include "contents/helpers/users.h"

namespace contents {


struct BaseContent : public cppcms::base_content {
    std::string lang;
    forms::SearchesSimple searchesSimple;
    UsersHelper userHelperContent;
    
};

}

#endif
