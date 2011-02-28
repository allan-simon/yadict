#include "models/Metas.h"
#include "models/Translations.h"

extern "C" {
#include "tato/kvlist.h"
}

namespace models {

/**
 *
 */
Metas::Metas() :
    logs(cppdb::session("sqlite3:db=../doc/sqlite3.db")) {

}
/**
 *
 */
bool Metas::add_meta(
    int wordId,
    std::string key,
    std::string value,
    int userId
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
            
        if (!tato_hyper_item_meta_set(word, key.c_str(), value.c_str())) {
            if(tato_hyper_item_meta_add(
                word,
                tato_hyper_db_common_str(tatoHyperDb, key.c_str()),
                value.c_str()
            )) {
                logs.insert_add_meta(
                    wordId,
                    std::string(word->lang->code),
                    key,
                    value,
                    userId
                );
                return true;
            }
        }

    }
    return false;
}
/**
 *
 */
bool Metas::edit_meta(
    int wordId, std::string key,
    std::string newValue,
    int userId
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
        const char* prevValueC = tato_hyper_item_meta_get(
            word,
            key.c_str()
        );
        if (prevValueC == NULL) {
            return false;
        }
        std::string prevValue(prevValueC);

        if(tato_hyper_item_meta_set(
            word,
            key.c_str(),
            newValue.c_str()
        )) {
            logs.insert_edit_meta(
                wordId,
                std::string(word->lang->code),
                key,
                newValue,
                userId,
                prevValue
            );

            return true;
        }
    }
    return false;
}

/**
 *
 */
bool Metas::remove_meta(
    int wordId,
    std::string key,
    int userId
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    std::string prevValue(
        tato_hyper_item_meta_get(
            word,
            key.c_str()
        )
    );



    if (word != NULL) {
        if( tato_hyper_item_meta_delete(
            word,
            key.c_str()
        )) {
            logs.insert_delete_meta(
                wordId,
                std::string(word->lang->code),
                key,
                prevValue,
                userId,
                true,
                0
            );

            return true;
        }
    }
    return false;
}



/**
 *
 */
bool Metas::has_meta(int wordId, std::string key) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
        return tato_hyper_item_meta_get(word, key.c_str()) != NULL;
    }
    return false;
}

/**
 *
 */
std::string Metas::get_meta_from_key(int wordId, std::string key) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
        return std::string(tato_hyper_item_meta_get(word, key.c_str())) ;
    }
    return "";
}

/**
 *
 */
MetasMap Metas::get_all_metas_of_word(int wordId) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);
    if (word == NULL) {
        return MetasMap();

    }
    return get_all_metas_of_word(word);
}


MetasMap Metas::get_all_metas_of_word(TatoHyperItem* item) {
    MetasMap metasMap;

	TatoKvListNode *it;
	TATO_KVLIST_FOREACH(item->metas, it) {
        metasMap[std::string(it->key)] = std::string(it->value);
    }
    return metasMap;
}



}// end of models::Metas

