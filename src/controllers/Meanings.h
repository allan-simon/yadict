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
        
        void add_to_word(std::string wordId);
        void add_to_word_treat();

        void add_def_in(
            std::string meaningIdStr, 
            std::string defLang, 
            std::string wordIdStr
        );

        void add_def_in_treat();



        void edit_def_in(
            std::string meaningIdStr, 
            std::string defLang, 
            std::string wordIdStr
        );

        void edit_def_in_treat();

        void show(
            std::string meaningIdStr,
            std::string wordIdStr
        );

        void delete_def_in(
            std::string meaningIdStr,
            std::string defLang
        );
        void delete_by_id(std::string meaningIdStr); 
        //

};
}

#endif


