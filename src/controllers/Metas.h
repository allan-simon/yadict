#ifndef CONTROLLERS_METAS_H
#define CONTROLLERS_METAS_H

#include "Controller.h"
#include "models/Metas.h"
#include "models/Words.h"

extern "C" {
#include "tato/hyperdb.h"
#include "tato/hyperitem.h"
#include "tato/list.h"
}


namespace controllers {

class Metas : public Controller {
    private:
        models::Metas metasModel;
        models::Words wordModel;
	public:
		Metas(cppcms::service &serv);

        // TODO Add something to view all defined metas (as we keep trace 
        // of them in the database

        void add(std::string wordId);
        void add_treat();

        //void edit(std::string wordId);
        //void edit_treat();

        //void delete_by_id_name(std::string wordId, std::string metaName);
};

} // End namespace

#endif

