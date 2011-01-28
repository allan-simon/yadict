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

}// end of models::Metas

