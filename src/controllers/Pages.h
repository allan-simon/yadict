#ifndef CONTROLLERS_PAGES_H
#define CONTROLLERS_PAGES_H

#include "Controller.h"

namespace controllers {

class Pages : public Controller {
	public:
		Pages(apps::Shanghainesedict& shdictApp);
		void homepage();
};

} // End namespace

#endif
