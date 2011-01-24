#include "models/Translations.h"

namespace models {
/**
 *
 */
Translations::Translations() {}

/**
 *
 */
bool Translations::add_to(
    int fromWordId,
    int translationRelId,
    int toWordId
) { 
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();

    // if the translation link already exists
    if (translationRelId > 0) {

        TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
        tato_hyper_db_relation_add_end(
            tatoHyperDb,
            translationRelId,
            toWordId
        );


    // if a translation does not already exist 
    // but the word exist 
    // NOTE we trust what is send by the request 
    // so yep maybe the request is crafted , but i don't think there's any
    // security threat there
    } else if (fromWordId > 0) {
        tato_hyper_db_relation_add(
            tatoHyperDb,
            fromWordId,
            toWordId,
            SHDICT_TRANSLATION_REL_FLAG,
            0
        );

    }
    return true;
}

}
