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
        void initContent(contents::BaseContent& content);
	public:
		Controller(cppcms::service &serv);
};

} // End namespace

#endif
