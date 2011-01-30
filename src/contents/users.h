#ifndef CONTENTS_USERS_H
#define CONTENTS_USERS_H

#include "contents/content.h"
#include "contents/forms/register.h"
#include "contents/forms/login.h"

namespace contents {

struct UsersRegisterNew : public BaseContent {
    forms::RegisterNewUser registerNewUser;
};

struct UsersLogin : public BaseContent {
    forms::LoginUser loginUser;
};



}

#endif

