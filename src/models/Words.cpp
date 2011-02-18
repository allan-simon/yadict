#include <sstream>
#include "models/Words.h"
#include "models/Translations.h"

namespace models {

/**
 *
 */
Words::Words() :
    logs(cppdb::session("sqlite3:db=../doc/sqlite3.db")) {

}

/**
 *
 */
TatoHyperItemFetcher* Words::get_word_with_id(int id) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, id);

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(1, 0);
    tato_hyper_item_fetcher_add(fetcher, word);

    return fetcher;
}

/**
 *
 */
TatoHyperItem* Words::get_word_with_lang_str(
    std::string lang,
    std::string str    
) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItem *word = NULL;
    TatoHyperItemLang *tatoLang= tato_hyper_db_lang_find(
        tatoHyperDb,
        lang.c_str()
    );
    if (tatoLang != NULL) {
        word = tato_hyper_item_lang_item_find(tatoLang, str.c_str());
    }
    /*
    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(1, 0);
    tato_hyper_item_fetcher_add(fetcher, word);
    */

    return word;
}



/**
 *
 */
TatoHyperItemFetcher* Words::get_words_with_str(std::string str) {
    return get_words_with_str(str, 10, 0);
}

TatoHyperItemFetcher* Words::get_words_with_str(std::string str, int size, int offset) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);
    tato_hyper_db_items_search(
        tatoHyperDb,
        fetcher,
        str.c_str()
    );

    return fetcher;
}


/**
 *
 */
TatoHyperItemFetcher* Words::get_all_words() {
    return get_all_words(10, 0);
}

TatoHyperItemFetcher* Words::get_all_words(int offset, int size) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);
    tato_hyper_db_items_get_seq(tatoHyperDb, fetcher);

    return fetcher;
}

/**
 *
 */
TatoHyperItemFetcher* Words::get_all_words_in_lang(std::string queryLang) {
    return get_all_words_in_lang(queryLang, 10, 0);
}

TatoHyperItemFetcher* Words::get_all_words_in_lang(
    std::string queryLang,
    int offset,
    int size
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);

    TatoHyperItemLang *tatoLang= tato_hyper_db_lang_find(
        tatoHyperDb,
        queryLang.c_str()
    );
    if (tatoLang != NULL) {
        tato_hyper_item_lang_items_get_seq(tatoLang, fetcher);
    }

    return fetcher;
}






/**
 *
 */
int Words::get_random_word_id() {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItem *randWord = tato_hyper_db_item_rand(tatoHyperDb);

    if (randWord == NULL) {
        return 0;
    } else {
        return randWord->id;
    }
}

/**
 *
 */
TatoHyperItemFetcher *Words::get_random_word() {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *randWord = tato_hyper_db_item_rand(tatoHyperDb);

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(1, 0);
    tato_hyper_item_fetcher_add(fetcher, randWord);

    return fetcher;
}

/**
 *
 */
TatoHyperItem* Words::add_word(
    std::string lang,
    std::string str,
    int userId
) {
    return add_word(lang, str, 0, userId);
}

TatoHyperItem* Words::add_word(
    std::string lang,
    std::string str,
    TatoHyperItemFlags flags,
    int userId
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    // TODO use exceptions when the word can is not added 
    // to signal either it is due to an internal error or because
    // we're trying to add a duplicate
    TatoHyperItem *newItem = tato_hyper_db_item_add(
        tatoHyperDb,
        lang.c_str(),
        str.c_str(),
        flags
    );
    
    if (newItem != NULL) {
        logs.insert_add_word(
            newItem->id,
            lang,
            str,
            userId
        );
    
    }
    return newItem;
}

/**
 *
 */
bool Words::edit_word(
    int id,
    std::string lang,
    std::string str,
    int userId
) {
    return edit_word(id, lang, str, 0, userId);
}

bool Words::edit_word(
    int id,
    std::string lang,
    std::string str,
    TatoHyperItemFlags flags,
    int userId
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 

    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, id);
    if (word == NULL) {
        return false;
    }
    std::string prevLang(word->lang->code);
    std::string prevText(word->str);

    // update the lang
    // TODO maybe it's worth to check if the lang has actually changed
    // as well as string and flag 
    TatoHyperItemLang *tatoLang = tato_hyper_db_lang_find(
        tatoHyperDb,
        lang.c_str()
    );
    if (tatoLang != NULL) {
        tato_hyper_item_lang_set(word, tatoLang);
    }
    //update the string 
    tato_hyper_item_str_set(word, str.c_str());

    //update the flag
    tato_hyper_item_flags_set(word, flags);

    //log it
    logs.insert_edit_word(
        word->id,
        lang,
        str,
        userId,
        prevLang,
        prevText
    );


    return true;
}

/**
 *
 */

bool Words::delete_word(int id, int userId) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 

    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, id);
    if (word == NULL) {
        return false;
    }
    //log it
    logs.insert_delete_word(
        word->id,
        std::string(word->lang->code),
        std::string(word->str),
        userId
    );

    return tato_hyper_db_item_delete(tatoHyperDb, id);
}

/**
 *
 */
int  Words::get_translation_relation(TatoHyperItem *word) {
    if (word == NULL) {
        return -1;
    }

    TatoHyperRelationsNode *it;
    TATO_HYPER_RELATIONS_FOREACH(word->startofs, it) {
        if (it->relation->type & SHDICT_TRANSLATION_REL_FLAG != 0) {
            return it->relation->id;
        }
    }
    return 0;
    
}

/**
 *
 */
MeaningsTranslationsMap Words::pack_translations(
    TatoHyperItemFetcher* fetcher,
    TranslationsMap & packedTransWithoutMeaning
) {
    
    MeaningsTranslationsMap meanTransMap;
    TranslationsMap packedTrans;

    if (fetcher->items[0] == NULL) {
        return meanTransMap; 
    }

	TatoHyperRelationsNode *it;
	TATO_HYPER_RELATIONS_FOREACH(fetcher->items[0]->startofs, it) {
        if (it->relation->type == SHDICT_TRANSLATION_REL_FLAG) {
		    TatoHyperItemsNode *it2;
            TATO_HYPER_ITEMS_FOREACH(it->relation->ends, it2) {
                std::string lang(it2->item->lang->code); 
                std::cout << "add " << it2->item->str << " in " << lang << std::endl;
                packedTrans[lang].insert(it2->item);
            }
            break;
        }

	}
    
    packedTransWithoutMeaning = packedTrans;
    
    // now we fill the Meanings => TranslationsMap
    
	TATO_HYPER_RELATIONS_FOREACH(fetcher->items[0]->startofs, it) {
        if (it->relation->type == SHDICT_MEANING_REL_FLAG ) {
		    TatoHyperItemsNode *it2;
            TranslationsMap tempTransMap; 
            TATO_HYPER_ITEMS_FOREACH(it->relation->ends, it2) {

                std::string lang(it2->item->lang->code); 
                // if the item is linked to the word with a "translation" relation
                if (packedTrans[lang].find(it2->item) != packedTrans[lang].end()) {
                    tempTransMap[lang].insert(it2->item);
                    packedTransWithoutMeaning[lang].erase(it2->item);
                }
            }
            meanTransMap[it->relation] = tempTransMap;
        }

	}

    return meanTransMap;

}

}// end of models::Words
