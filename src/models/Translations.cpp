#include "models/Translations.h"
#include "models/Words.h"

namespace models {
/**
 *
 */
Translations::Translations() {}

/**
 *
 */
bool Translations::add_translation_to_meaning(
    int meaningId,
    int fromWordId,
    std::string transText,
    std::string transLang
) {
    TatoHyperItem *transWord = add_translation_to_word(
        fromWordId,
        0,
        transText,
        transLang
    );

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER() ; 
    TatoHyperRelation* relation = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );

    if (relation->type == SHDICT_MEANING_REL_FLAG) {
        tato_hyper_db_relation_add_end(
            tatoHyperDb,
            meaningId,
            transWord->id
        );
    } else {
        return false;
    }
    

    return true;
}

/**
 *
 */
TatoHyperItem* Translations::add_translation_to_word(
    int fromWordId,
    int transRelId,
    std::string transText,
    std::string transLang
) {
    
    models::Words wordsModel;
    TatoHyperItem *translation = wordsModel.add_word(
        transLang,
        transText
    );

    // if we were not able to add the word
    // we test if the word didn't exist before
    // TODO with exception we should be able to avoid this
    // by adding a field with the TatoHyperItem in the exception
    // raised by addWord
    if (translation == NULL) {
       translation = wordsModel.get_word_with_lang_str(transLang, transText); 
    }

    if (translation != NULL) {
        add_one_way_link(
            fromWordId,
            transRelId,
            translation->id
        );

        add_one_way_link(
            translation->id,
            0,
            fromWordId
        );
    };

    return translation;
}

/**
 *
 */
bool Translations::add_one_way_link(
    int fromWordId,
    int translationRelId,
    int toWordId
) { 
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER() ; 

    // if the translation link already exists
    if (translationRelId > 0) {

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
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
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

/**
 *
 */
bool Translations::remove_from_meaning(int transId, int meaningId) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperRelation* relation = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );

    if (relation->type == SHDICT_MEANING_REL_FLAG) {
        tato_hyper_db_relation_remove_end(
            tatoHyperDb,
            meaningId,
            transId
        );
    } else {
        return false;
    }
    
    return true;
}

}
