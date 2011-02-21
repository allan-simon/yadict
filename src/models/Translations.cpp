#include "models/Translations.h"
#include "models/Words.h"

namespace models {
/**
 *
 */
Translations::Translations() :
    logs(cppdb::session("sqlite3:db=../doc/sqlite3.db")) {

}


/**
 *
 */
bool Translations::add_translation_to_meaning(
    int meaningId,
    int fromWordId,
    std::string transText,
    std::string transLang,
    int userId
) {
    results::Word transWord = add_translation_to_word(
        fromWordId,
        0,
        transText,
        transLang,
        userId
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
            transWord.id
        );
    } else {
        return false;
    }
    

    return true;
}

/**
 *
 */
results::Word Translations::add_translation_to_word(
    int fromWordId,
    int transRelId,
    std::string transText,
    std::string transLang,
    int userId
) {
    
    models::Words wordsModel;
    results::Word transWord = wordsModel.add_word(
        transLang,
        transText,
        userId
    );

    // if we were not able to add the word
    // we test if the word didn't exist before
    // TODO with exception we should be able to avoid this
    // by adding a field with the TatoHyperItem in the exception
    // raised by addWord
    if (transWord.exists()) {
       transWord = wordsModel.get_word_with_lang_str(transLang, transText); 
    }

    if (transWord.exists()) {
        add_one_way_link(
            fromWordId,
            transRelId,
            transWord.id
        );

        add_one_way_link(
            transWord.id,
            0,
            fromWordId
        );

        logs.insert_add_trans(
            fromWordId,
            transWord.id,
            userId
        );
    };

    return transWord;
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
        if (origWord == NULL) {
            return false;
        }
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
 *
 */
bool Translations::remove_from_words(
    int wordId1,
    int wordId2,
    int userId
) {
    if (remove_one_way(wordId1,wordId2) && remove_one_way(wordId2,wordId1)) {

        logs.insert_delete_trans(
            wordId1,
            wordId2,
            userId,
            true,
            0
        );
        return true;
    } 
    return false;
}

/**
 *
 */
bool Translations::remove_one_way(
    int transId,
    int origId
) {
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


/**
 *
 */
TransVector Translations::get_all_trans_id(int wordId) {
    TransVector transVector;

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);
    if (word == NULL) {
        return transVector;
    }

	TatoHyperRelationsNode *it;
	TATO_HYPER_RELATIONS_FOREACH(word->startofs, it) {
        if (it->relation->type == SHDICT_TRANSLATION_REL_FLAG) {
		    TatoHyperItemsNode *it2;
            TATO_HYPER_ITEMS_FOREACH(it->relation->ends, it2) {
                transVector.push_back(it2->item->id);
            }
            break;
        }
    }
    return transVector;
}

}
