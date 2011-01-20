#include <sstream>
#include "models/Words.h"

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

}// end of models::Words
