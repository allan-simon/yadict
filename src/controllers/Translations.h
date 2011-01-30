#ifndef SHDICT_CONTROLLERS_TRANSLATIONS_H
#define SHDICT_CONTROLLERS_TRANSLATIONS_H

#include "Controller.h"
#include "models/Translations.h"

extern "C" {
#include "tato/hyperdb.h"
#include "tato/hyperitem.h"
#include "tato/hyperrelation.h"
#include "tato/hyperrelations.h"
#include "tato/list.h"
}


namespace controllers {

class Translations : public Controller {
    private:
        models::Translations transModel;
	public:
		Translations(cppcms::service &serv);
        
        void add_to(std::string origWordId);
        void add_to_treat();

        void remove(std::string transIdStr, std::string origIdStr);

        //
        void link(std::string origWordId, std::string transWordId);

};
}

#endif

