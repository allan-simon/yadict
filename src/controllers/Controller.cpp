#include "Controller.h"
#include "shanghainesedict.h"
#include <cppcms/session_interface.h>

namespace controllers {

Controller::Controller(cppcms::service &serv) : cppcms::application(serv) {


}

void Controller::init_content(contents::BaseContent& content) {
    
    response().content_encoding("UTF-8");
    response().set_content_header("text/html; charset=utf-8");

    // TODO seems context.locale return always a 2 letter code
    // TODO also need to check the locale send by the user navigator
    // without forgetting the navigator always send more than one locale
    // in order to have some fallbacks
    if (session().is_set("lang")) {

        content.lang = session()["lang"];
    } else {

        content.lang = "eng";
    }
    std::cout << "user name: " << session()["name"] << std::endl;
    if (session().is_set("name")) {
        content.userHelperContent.username = session()["name"];
    }
}

bool Controller::is_logged() {
    return !session()["name"].empty();
}

void Controller::go_back_to_previous_page() {
    response().set_redirect_header(
        request().http_referer()
    );
}

} // End namespace

