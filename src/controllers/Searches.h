#ifndef CONTROLLERS_SEARCHES_H
#define CONTROLLERS_SEARCHES_H

#include "Controller.h"
#include "models/Searches.h"

extern "C" {
#include "tato/hyperdb.h"
#include "tato/hyperitem.h"
#include "tato/list.h"
}


namespace controllers {

class Searches : public Controller {
    private:
        models::Searches searchesModel;
	public:
		Searches(cppcms::service &serv);
        void simple_treat();

        void show_result(std::string query, std::string lang);
        void show_result(
            std::string query,
            std::string lang,
            std::string offset,
            std::string size
        );

};

}; // End namespace

#endif

