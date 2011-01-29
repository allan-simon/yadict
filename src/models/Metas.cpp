#include "models/Metas.h"
#include "models/Translations.h"

namespace models {

/**
 *
 */
Metas::Metas() {}

/**
 *
 */
bool Metas::add_meta(
    int wordId,
    std::string key,
    std::string value
) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::get_instance("")->get_database_pointer();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
            
        if (!tato_hyper_item_meta_set(word, key.c_str(), value.c_str())) {
            return tato_hyper_item_meta_add(
                word,
                tato_hyper_db_common_str(tatoHyperDb, key.c_str()),
                value.c_str()
            );
        }

    }
    return false;
}
/**
 *
 */
bool Metas::edit_meta(int wordId, std::string key, std::string newValue) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::get_instance("")->get_database_pointer();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
        return tato_hyper_item_meta_set(
            word,
            key.c_str(),
            newValue.c_str()
        );
    }
    return false;
}

/**
 *
 */
bool Metas::remove_meta(int wordId, std::string key) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::get_instance("")->get_database_pointer();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
        return tato_hyper_item_meta_delete(
            word,
            key.c_str()
        );
    }
    return false;
}



/**
 *
 */
bool Metas::has_meta(int wordId, std::string key) {

    TatoHyperDb *tatoHyperDb = TatoHyperDB::get_instance("")->get_database_pointer();
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

    TatoHyperDb *tatoHyperDb = TatoHyperDB::get_instance("")->get_database_pointer();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);

    if (word != NULL) {
        return std::string(tato_hyper_item_meta_get(word, key.c_str())) ;
    }
    return "";
}

}// end of models::Metas

