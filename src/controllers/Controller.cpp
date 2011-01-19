#include "Controller.h"
#include "shanghainesedict.h"
#include <cppcms/session_interface.h>

namespace controllers {

Controller::Controller(apps::Shanghainesedict &shdictapp) :
	cppcms::application(shdictapp.service()),
	shdictapp(shdictapp) {

	configContent.css_path = settings().get<std::string>("shanghainesedict.css");
	configContent.img_path = settings().get<std::string>("shanghainesedict.img");
    configContent.web_path = settings().get<std::string>("shanghainesedict.web");
}

void Controller::initContent(contents::BaseContent& content) {
    response().content_encoding("UTF-8");
    response().set_content_header("text/html; charset=utf-8");
    content.config = configContent;
    std::locale current_locale = context().locale();
    booster::locale::info const &inf = std::use_facet<booster::locale::info>(
        current_locale
    );
    content.lang = inf.language();
    if (session().is_set("name")) {
        content.isAuthenticated = true;
    } else {
        content.isAuthenticated = false;
    }
}

} // End namespace

