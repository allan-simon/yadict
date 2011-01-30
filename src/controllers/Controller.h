#ifndef CONTROLLERS_CONTROLLER_H
#define CONTROLLERS_CONTROLLER_H

#include <cppcms/json.h>
#include <cppcms/application.h>
#include <cppcms/url_dispatcher.h>

#include "contents/content.h"

namespace apps {
class Shanghainesedict;
}
namespace controllers {

class Controller : public cppcms::application {
	protected:
        void init_content(contents::BaseContent& content);
        bool is_logged();
        void go_back_to_previous_page();
	public:
		Controller(cppcms::service &serv);
};

} // End namespace

#endif
