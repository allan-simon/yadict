#ifndef CONTENTS_HELPER_USERS_H
#define CONTENTS_HELPER_USERS_H


#include "contents/Config.h"
#include "helpers.h"

namespace contents {

struct UsersHelper : public Helpers {
    
        std::string username;

        public:
            bool is_logged() {
                return !username.empty();
            }

};

};
#endif

