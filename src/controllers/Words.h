#ifndef CONTROLLERS_WORDS_H
#define CONTROLLERS_WORDS_H

#include "Controller.h"
#include "models/Words.h"

extern "C" {
#include "tato/hyperdb.h"
#include "tato/hyperitem.h"
#include "tato/list.h"
}


namespace controllers {

class Words : public Controller {
    private:
        models::Words wordModel;
	public:
		Words(apps::Shanghainesedict& shdictapp);
		void show(std::string word_str);
        void show_random();
        void show_all();
        void show_all(int size, int offset);
        void show_all(std::string lang);
        void show_all(std::string lang, int size, int offset);
};

} // End namespace

#endif

