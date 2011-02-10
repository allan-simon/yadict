#ifndef YADICT_USERS_LOGIN_FORM_H
#define YADICT_USERS_LOGIN_FORM_H

#include <cppcms/form.h>
using namespace cppcms;

namespace forms {

	struct LoginUser : public cppcms::form {
        widgets::hidden previousUrl;
		widgets::text username;
		widgets::password password;
		widgets::checkbox rememberMe;
		widgets::submit submit;
		
		LoginUser() {
            previousUrl.name("previousurl");
            previousUrl.value("");

			username.name("username");
			username.message("Username : ");
			username.attributes_string("class=\"input text\"");

			password.name("password");
			password.message("Password : ");

			rememberMe.name("rememberMe");
			rememberMe.message("Remember Me");

            

			submit.name("submit");
			submit.value("Log in");

			*this + previousUrl + username + password + rememberMe + submit;

			username.non_empty();
			password.non_empty();
		}
		
		virtual bool validate() {
			if ( ! form::validate()) {
                // TODO complete this
				return false;
			}
			return true;
		}
		
	};

}
#endif
