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

        TatoHyperItem *origWord= tato_hyper_db_item_find(tatoHyperDb, fromWordId);
        TatoHyperRelationsNode *it;
        TATO_HYPER_RELATIONS_FOREACH(origWord->startofs, it) {
            if (it->relation->type == SHDICT_TRANSLATION_REL_FLAG) {
                tato_hyper_db_relation_add_end(
                    tatoHyperDb,
                    it->relation->id,
                    toWordId
                );
                return true;
            }

        }

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

/**
 *
 */
bool Translations::remove(int transId, int origId) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
    TatoHyperItem *origWord= tato_hyper_db_item_find(tatoHyperDb, origId);
    if (origWord == NULL) {
        return false;
    }

	TatoHyperRelationsNode *it;
	TATO_HYPER_RELATIONS_FOREACH(origWord->startofs, it) {
        if (it->relation->type == SHDICT_TRANSLATION_REL_FLAG) {
            tato_hyper_db_relation_remove_end(
                tatoHyperDb,
                it->relation->id,
                transId
            );
            return true;
        }

	}
    return false;
}
}
