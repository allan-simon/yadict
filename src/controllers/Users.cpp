#include <vector>
#include <cppcms/session_interface.h>

#include "Controller.h"
#include "controllers/Users.h"

#include "contents/users.h"
#include "models/Users.h"

namespace controllers {

Users::Users(cppcms::service &serv) :
    Controller(serv),
    userModel(cppdb::session("sqlite3:db=../doc/sqlite3.db"))
{

    cppcms::url_dispatcher* d = &dispatcher();
    d->assign("/register-new", &Users::register_new, this);
    d->assign("/register-new-treat", &Users::register_new_treat, this);
  	d->assign("/login", &Users::login, this);
  	d->assign("/login-treat", &Users::login_treat, this);
  	d->assign("/logout", &Users::logout, this);
    //tatoapp.dispatcher().assign("/users/all((/\\d+)?)", &Users::listMembers, this, 1);
}


void Users::register_new() {
    contents::UsersRegisterNew c;
    init_content(c);
    

    render("users_register_new", c);
}

void Users::register_new_treat() {
    contents::UsersRegisterNew c;
    init_content(c);
    c.registerNewUser.load(context());

    if(c.registerNewUser.validate()) {
        if (
            userModel.add_user(
                c.registerNewUser.username.value(),
                c.registerNewUser.password.value(),
                c.registerNewUser.email.value()
            )
        ) {
            response().set_redirect_header("/" + c.lang);
            session()["name"] = c.registerNewUser.username.value();
            session().save();
            std::cout << "user name: " << session()["name"] << std::endl;
            return;
        }
    }
    
    response().set_redirect_header(
        request().http_referer()
    );
}


void Users::login() {
    contents::UsersLogin c;
    init_content(c);

    render("users_login", c);
}

void Users::login_treat() {

    contents::UsersLogin c;
    c.loginUser.load(context());

    if (
        userModel.is_login_correct(
            c.loginUser.username.value(),
            c.loginUser.password.value()
        )
    ) {
        session()["name"] = c.loginUser.username.value();
        session().save();
        response().set_redirect_header("/" + c.lang);

    } else {
        response().set_redirect_header(
            request().http_referer()
        );
    }
}

void Users::logout() {
    session().clear();
    response().set_redirect_header(
        request().http_referer()
    );
}
/*
void Users::list_members(std::string page) {
    int intPage;

    // check if there is a page in argument
    if (page.compare("") == 0) {
        intPage = 1;
    }
    else {
        // convert page from string to integer
        std::string realPage = page.substr(1);  // remove the slash in "/pageNumber"
        intPage = atoi(realPage.c_str());
    }

    contents::AllUsers c;
    init_content(c);
    c.listOfMembers = userModel.getAllUsers();
    render("allusers", c);
}
*/
} // End namespace
