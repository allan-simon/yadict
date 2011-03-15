#include "TatoHyperDB.h"
#include "SearchEngine.h"

#include <cstring>
/**
 *
 */
TatoHyperDb *TatoHyperDB::get_database_pointer() {
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
void TatoHyperDB::dump(std::string path) {
    tato_hyper_db_dump(tatoHyperDb, path.c_str());
}
 

/**
 *
 */
TatoHyperDB::~TatoHyperDB() {
    tato_hyper_db_free(tatoHyperDb);
}


/**
 *
 */
void TatoHyperDB::feed_search_engine() {
	TatoTreeIntNode *it = NULL;
    TatoHyperItem *item = NULL;
	RB_FOREACH(it, TatoTreeInt_, tatoHyperDb->items) {
        item = (TatoHyperItem*) it->value;
        SearchEngine::get_instance()->add_word(
            item->id,
            std::string(item->str),
            std::string(item->lang->code)
        );

        TatoKvListNode *itkv;
        TATO_KVLIST_FOREACH(item->metas, itkv) {
            SearchEngine::get_instance()->add_meta(
                item->id,
                std::string(itkv->key),
                std::string(itkv->value),
                std::string(item->lang->code)
            );
        }
	}
}
