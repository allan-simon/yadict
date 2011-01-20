#include "TatoHyperDB.h"


/**
 *
 */
TatoHyperDb *TatoHyperDB::getDatabasePointer() {
    return tatoHyperDb;
}


/**
 *
 *
 */

TatoHyperDB::TatoHyperDB(std::string data_path) {
    tatoHyperDb = tato_hyper_db_new();
    tato_hyper_db_load(tatoHyperDb, data_path.c_str());
}

 

/**
 *
 */
TatoHyperDB::~TatoHyperDB() {
    tato_hyper_db_free(tatoHyperDb);
}

