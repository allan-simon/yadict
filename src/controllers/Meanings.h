#ifndef SHDICT_CONTROLLERS_MEANINGS_H
#define SHDICT_CONTROLLERS_MEANINGS_H

#include "Controller.h"
#include "models/Meanings.h"

extern "C" {
#include "tato/hyperdb.h"
#include "tato/hyperitem.h"
#include "tato/hyperrelation.h"
#include "tato/hyperrelations.h"
#include "tato/list.h"
}


namespace controllers {

class Meanings : public Controller {
    private:
        models::Meanings meaningsModel;
	public:
		Meanings(cppcms::service &serv);
        
        void add_to_word(std::string origWordId);
        void add_to_word_treat();

        void edit(
            std::string meaningIdStr, 
            std::string meaningLang, 
            std::string origWordIdStr
        );
        void edit_treat();

        void delete_by_id(std::string meaningIdStr); 
        //

};
}

#endif


