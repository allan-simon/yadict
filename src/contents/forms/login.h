#ifndef YADICT_USERS_LOGIN_FORM_H
#define YADICT_USERS_LOGIN_FORM_H

#include <cppcms/form.h>

namespace forms {

	struct LoginUser : public cppcms::form {
		cppcms::widgets::text username;
		cppcms::widgets::password password;
		cppcms::widgets::checkbox rememberMe;
		cppcms::widgets::submit submit;
		
		LoginUser() {
			username.name("username");
			username.message("Username : ");
			username.attributes_string("class=\"input text\"");

			password.name("password");
			password.message("Password : ");

			rememberMe.name("rememberMe");
			rememberMe.message("Remember Me");

			submit.name("submit");
			submit.value("Log in");

			*this + username + password + rememberMe + submit;

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
