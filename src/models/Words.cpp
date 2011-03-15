#include <sstream>
#include "models/Words.h"
#include "models/Translations.h"
#include "models/SearchEngine.h"

namespace models {

/**
 *
 */
Words::Words() :
    // TODO rather use a path in config
    logs(cppdb::session("sqlite3:db=../doc/sqlite3.db")) {

}

/**
 *
 */
results::Word Words::get_word_with_id(int id) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItem *wordItem= tato_hyper_db_item_find(tatoHyperDb, id);

    return word_from_item(wordItem);
}

/**
 *
 */
results::Word Words::get_word_with_lang_str(
    std::string lang,
    std::string str    
) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItem *wordItem = NULL;
    TatoHyperItemLang *tatoLang= tato_hyper_db_lang_find(
        tatoHyperDb,
        lang.c_str()
    );
    if (tatoLang == NULL) {
        results::Word word;
        return word;
    }

    wordItem = tato_hyper_item_lang_item_find(tatoLang, str.c_str());
     
    return word_from_item(wordItem);
}



/**
 *
 */
results::WordsVector Words::get_words_with_str(std::string str) {
    return get_words_with_str(str, 10, 0);
}

results::WordsVector Words::get_words_with_str(std::string str, int size, int offset) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);
    tato_hyper_db_items_search(
        tatoHyperDb,
        fetcher,
        str.c_str()
    );

    results::WordsVector wordsVector(fetcher->size);
    for(int i = 0; i < fetcher->size ; i++) {
        wordsVector[i] = word_from_item(fetcher->items[i]);
    }
    wordsVector.offset = offset;
    wordsVector.maxsize = fetcher->max_size;

    tato_hyper_item_fetcher_free(fetcher);

    return wordsVector;
}


/**
 *
 */
results::WordsVector Words::get_all_words() {
    return get_all_words(10, 0);
}

results::WordsVector Words::get_all_words(int offset, int size) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);
    tato_hyper_db_items_get_seq(tatoHyperDb, fetcher);

    results::WordsVector wordsVector(fetcher->size);
    for(int i = 0; i < fetcher->size ; i++) {
        wordsVector[i] = results::Word(fetcher->items[i]);
    }
    wordsVector.offset = offset;
    wordsVector.maxsize = fetcher->max_size;

    tato_hyper_item_fetcher_free(fetcher);

    return wordsVector;
}

/**
 *
 */
results::WordsVector Words::get_all_words_in_lang(std::string queryLang) {
    return get_all_words_in_lang(queryLang, 10, 0);
}

results::WordsVector Words::get_all_words_in_lang(
    std::string queryLang,
    int offset,
    int size
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();


    TatoHyperItemLang *tatoLang= tato_hyper_db_lang_find(
        tatoHyperDb,
        queryLang.c_str()
    );
    if (tatoLang == NULL) {
        return results::WordsVector(0);
    }

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);
    tato_hyper_item_lang_items_get_seq(tatoLang, fetcher);

    results::WordsVector wordsVector(fetcher->size);
    for(int i = 0; i < fetcher->size ; i++) {
        wordsVector[i] = results::Word(fetcher->items[i]);
    }
    wordsVector.offset = offset;
    wordsVector.maxsize = fetcher->max_size;

    tato_hyper_item_fetcher_free(fetcher);

    return wordsVector;
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
results::Word Words::get_random_word() {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *randWord = tato_hyper_db_item_rand(tatoHyperDb);

    return results::Word(randWord);
}

/**
 *
 */
results::Word Words::add_word(
    std::string lang,
    std::string str,
    int userId
) {
    return add_word(lang, str, 0, userId);
}

results::Word Words::add_word(
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

        SearchEngine::get_instance()->add_word(
            newItem->id,
            str,
            lang
        );
    
    }
    return results::Word(newItem);
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

int  Words::get_translation_relation(int id) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, id);
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
results::Word Words::word_from_item(TatoHyperItem* item) {
    results::Word word(item);
    if (!word.exists()) {
        return word;
    }
    models::Metas metasModel;
    word.metas = metasModel.get_all_metas_of_word(item); 
    word.meanTransMap = pack_translations(item, word.transMap);
   
    return word; 
}

/**
 *
 */
MeaningsTransMap Words::pack_translations(
    TatoHyperItem* item,
    TransMap & packedTransWithoutMeaning
) {
    
    MeaningsTransMap meanTransMap;
    TransMap packedTrans;

    if (item == NULL) {
        return meanTransMap; 
    }

	TatoHyperRelationsNode *it;
	TATO_HYPER_RELATIONS_FOREACH(item->startofs, it) {
        if (it->relation->type == SHDICT_TRANSLATION_REL_FLAG) {
		    TatoHyperItemsNode *it2;
            TATO_HYPER_ITEMS_FOREACH(it->relation->ends, it2) {
                std::string lang(it2->item->lang->code); 
                packedTrans[lang].insert(it2->item);
            }
            break;
        }

	}
    
    packedTransWithoutMeaning = packedTrans;
    
    // now we fill the Meanings => TransMap
    
    models::Meanings meaningsModel;
    
	TATO_HYPER_RELATIONS_FOREACH(item->startofs, it) {
        if (it->relation->type == SHDICT_MEANING_REL_FLAG ) {
		    TatoHyperItemsNode *it2;
            TransMap tempTransMap; 

            results::Meaning tempMeaning = meaningsModel.get_meaning_by_id(
                it->relation->id
            );
            
            TATO_HYPER_ITEMS_FOREACH(it->relation->ends, it2) {

                std::string lang(it2->item->lang->code); 
                // if the item is linked to the word with a "translation" relation
                if (packedTrans[lang].find(it2->item) != packedTrans[lang].end()) {
                    // we do that to inform the meaning that it does have
                    // translations in that language though the definition
                    // is missing
                    if (tempMeaning.defsMap.find(lang) == tempMeaning.defsMap.end()) {
                         tempMeaning.defsMap[lang] = "";
                    }
                    tempTransMap[lang].insert(it2->item);
                    packedTransWithoutMeaning[lang].erase(it2->item);
                }
            }

            meanTransMap[tempMeaning] = tempTransMap;
        }

	}

    return meanTransMap;

}

}// end of models::Words
