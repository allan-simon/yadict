#include <sstream>
#include "models/Words.h"
#include "models/Translations.h"

namespace models {

/**
 *
 */
Words::Words() {}

/**
 *
 */
TatoHyperItemFetcher* Words::getWordWithId(int id) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, id);

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(1, 0);
    tato_hyper_item_fetcher_add(fetcher, word);

    return fetcher;
}

/**
 *
 */
TatoHyperItem* Words::getWordWithLangStr(
    std::string lang,
    std::string str    
) {

    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
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
TatoHyperItemFetcher* Words::getWordsWithStr(std::string str) {
    return getWordsWithStr(str, 10, 0);
}

TatoHyperItemFetcher* Words::getWordsWithStr(std::string str, int size, int offset) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);
    tato_hyper_db_items_search(tatoHyperDb, fetcher, str.c_str());

    return fetcher;
}


/**
 *
 */
TatoHyperItemFetcher* Words::getAllWords() {
    return getAllWords(10, 0);
}

TatoHyperItemFetcher* Words::getAllWords(int offset, int size) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);
    tato_hyper_db_items_get_seq(tatoHyperDb, fetcher);

    return fetcher;
}




/**
 *
 */
int Words::getRandomWordId() {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
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
TatoHyperItemFetcher *Words::getRandomWord() {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
    TatoHyperItem *randWord = tato_hyper_db_item_rand(tatoHyperDb);

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(1, 0);
    tato_hyper_item_fetcher_add(fetcher, randWord);

    return fetcher;
}

/**
 *
 */
TatoHyperItem* Words::addWord(std::string lang, std::string str) {
    return addWord(lang, str, 0);
}

TatoHyperItem* Words::addWord(
    std::string lang,
    std::string str,
    TatoHyperItemFlags flags
) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
    // TODO use exceptions when the word can is not added 
    // to signal either it is due to an internal error or because
    // we're trying to add a duplicate
    return tato_hyper_db_item_add(
        tatoHyperDb,
        lang.c_str(),
        str.c_str(),
        flags
    );
}

/**
 *
 */
bool Words::editWord(int id, std::string lang, std::string str) {
    return editWord(id, lang, str, 0);
}

bool Words::editWord(
    int id,
    std::string lang,
    std::string str,
    TatoHyperItemFlags flags
) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, id);
    if (word != NULL) {
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
    }
    // TODO handle errors
    return true;
}

/**
 *
 */

bool Words::deleteWord(int id) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
    return tato_hyper_db_item_delete(tatoHyperDb, id);
}

/**
 *
 */
int  Words::getTranslationRelation(TatoHyperItem *word) {
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
TranslationsMap Words::packTranslations(
    TatoHyperItemFetcher* fetcher
) {
    TranslationsMap packedTrans;
    if (fetcher->items[0] == NULL) {
        return packedTrans; 
    }
	

	TatoHyperRelationsNode *it;
	TATO_HYPER_RELATIONS_FOREACH(fetcher->items[0]->startofs, it) {
        if (it->relation->type == SHDICT_TRANSLATION_REL_FLAG) {
		    TatoHyperItemsNode *it2;
            TATO_HYPER_ITEMS_FOREACH(it->relation->ends, it2) {
                std::string lang(it2->item->lang->code); 
                std::cout << "add " << it2->item->str << " in " << lang << std::endl;
                packedTrans[lang].push_front(it2->item);
            }
            break;
        }

	}
    return packedTrans;

}

}// end of models::Words
