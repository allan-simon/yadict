#ifndef CONTROLLERS_USERS_H
#define CONTROLLERS_USERS_H

#include "Controller.h"
#include "models/Users.h"

namespace controllers {

class Users : public Controller {
    private:
        models::Users userModel;
	public:

		Users(cppcms::service &serv);
        void register_new();
        void register_new_treat();
		void login();
		void login_treat();
        void logout();
        /*
        void list_members();
        void list_members(std::string offset, std::string size);
        */
};

} // End namespace

#endif
