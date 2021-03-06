#ifndef CONTROLLERS_WORDS_H
#define CONTROLLERS_WORDS_H

#include "Controller.h"
#include "models/Words.h"

extern "C" {
#include "tato/hyperitem.h"
#include "tato/list.h"
}


namespace controllers {

class Words : public Controller {
    private:
        models::Words wordModel;
	public:
		Words(cppcms::service &serv);
		void show(std::string wordStr);
		void show_in(std::string wordStr, std::string wordLang);
        void show_random();
        
        void show_all();
        void show_all(std::string offset, std::string size);

        void show_all_in(std::string lang);
        void show_all_in(std::string lang, std::string offset, std::string size);

        void show_all_langs_filter_treat();

        void add();
        void add_treat();

        void edit(std::string wordId);
        void edit_treat();

        void delete_by_id(std::string wordId);

        void startdict_export(
            std::string src,
            std::string dest
        );
};

} // End namespace

#endif

