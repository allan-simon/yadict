#ifndef CONTENTS_BASE_CONTENT_H
#define CONTENTS_BASE_CONTENT_H

#include <cppcms/view.h>
#include "forms/login_form.h"
#include "forms/search.h"
#include "contents/Config.h"

namespace contents {


struct BaseContent : public cppcms::base_content {
    std::string lang;
    bool isAuthenticated;
    login_form login;
    forms::SearchesSimple searchesSimple;
};

}

#endif
