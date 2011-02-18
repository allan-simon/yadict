#ifndef CONTROLLERS_CONTROLLER_H
#define CONTROLLERS_CONTROLLER_H

#include <cppcms/json.h>
#include <cppcms/application.h>
#include <cppcms/url_dispatcher.h>

#include "contents/content.h"

#define CHECK_PERMISSION_OR_GO_TO_LOGIN() \
    if (!check_permission()) {\
       return;\
    }

namespace apps {
class Shanghainesedict;
}
namespace controllers {

class Controller : public cppcms::application {
	protected:
        void init_content(contents::BaseContent& content);
        bool is_logged();
        bool check_permission();
        void go_back_to_previous_page();
        std::string get_interface_lang();
        int get_current_user_id();
	public:
		Controller(cppcms::service &serv);
};

} // End namespace

#endif
