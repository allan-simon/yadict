#ifndef CONTROLLERS_LOGS_H
#define CONTROLLERS_LOGS_H

#include "Controller.h"
#include "models/Logs.h"

namespace controllers {

class Logs : public Controller {
    private:
        models::Logs logsModel;
	public:
		Logs(cppcms::service &serv);

        void show_last_words();
        void show_last_metas();
        //void show_last_in(std::string langFilter);

};

    

}; // End namespace

#endif


