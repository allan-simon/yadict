#ifndef SHDICT_REGISTER_FORM_H
#define SHDICT_REGISTER_FORM_H

#include <cppcms/form.h>
#include "contents/content.h"

namespace forms {

struct RegisterNewUser : public cppcms::form {
    cppcms::widgets::text username;
    cppcms::widgets::password password;
    cppcms::widgets::email email;
    cppcms::widgets::text quiz;
    cppcms::widgets::checkbox termsOfUse;
    cppcms::widgets::submit submit;

    RegisterNewUser() {
        *this + username + password + email + quiz + termsOfUse + submit;
        
        username.name("username");
        password.name("password");
        email.name("email");
        quiz.name("quiz");
        termsOfUse.name("termsofuse");


        submit.value("Register");
        submit.id("registerButton");

        username.non_empty();
        password.non_empty();

    }

    virtual bool validate() {
        return form::validate() &&
            termsOfUse.value() && //should have check the terms of use
            // the "captcha" test is to input the first five letters of 
            // the email so we test if it's correct
            email.value().substr(0,5).compare(quiz.value()) == 0; 
    }
};

}

#endif
