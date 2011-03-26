#include "TatoHyperDB.h"
#include "SearchEngine.h"
#include "Relations.h"

#include <cstring>
#include <fstream>
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

/**
 *
 */
void TatoHyperDB::startdict_export(
    std::string langSource,
    std::string langDest
) {
	TatoHyperItemLang *tatoLangSrc = tato_hyper_db_lang_find_or_create(
        tatoHyperDb,
        langSource.c_str()
    );
	TatoHyperItemLang *tatoLangDest = tato_hyper_db_lang_find_or_create(
        tatoHyperDb,
        langDest.c_str()
    );

    if (tatoLangSrc == NULL || tatoLangDest == NULL) {
        return;
    }
    
    std::string fileName = langSource + "_" +langDest +".dict" ;
    std::ofstream stardictFile;
    stardictFile.open(fileName.c_str(), std::ios::trunc);

    TatoTreeStrNode *it = NULL;
    TATO_TREE_STR_FOREACH(tatoLangSrc->index, it) {
        TatoHyperItem *item = (TatoHyperItem*) it->value;
        if (tato_hyper_item_related_with(item, tatoLangDest)) {

            TatoHyperRelationsNode *it;
            stardictFile << item->str;

            TATO_HYPER_RELATIONS_FOREACH(item->startofs, it) {
                if (it->relation->type == SHDICT_TRANSLATION_REL_FLAG) {
                    stardictFile << "\t";    

                    TatoHyperItemsNode *it2;
                    TATO_HYPER_ITEMS_FOREACH(it->relation->ends, it2) {
                        if (it2->item->lang == tatoLangDest) {
                            //const char* pinyin = tato_hyper_item_meta_get(it2->item,"pinyin");
                            
                            stardictFile << it2->item->str ;
                            /*
                            if (pinyin != NULL) {
                                stardictFile << " (" << pinyin << ")";
                            }
                            */
                            stardictFile << "\\n";
                        }
                    }
                    stardictFile << "\n";
                    break;
                }

            }
         
        }
    }
    stardictFile.close();

}
