#include "models/Searches.h"
#include "models/Translations.h"

namespace models {

/**
 *
 */
Searches::Searches() {}



results::WordsVector Searches::simple(std::string query, std::string lang) {
    return simple(query, lang, 10, 0);
}


results::WordsVector Searches::simple(
    std::string query,
    std::string lang,
    int size,
    int offset
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER();
    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);

    results::WordsVector wordsVector(fetcher->size);

    TatoHyperItemLang *tatoLang = NULL;
    if (!lang.empty()) {
        tatoLang = tato_hyper_db_lang_find(tatoHyperDb, lang.c_str());
        if (tatoLang != NULL) {

            models::Words wordsModel;

            tato_hyper_item_lang_items_suggest_pagi(
                tatoLang,
                fetcher,
                query.c_str()
            );

            for(int i = 0; i < fetcher->size ; i++) {
                wordsVector.push_back(
                    wordsModel.word_from_item(fetcher->items[i])
                );
            }
            wordsVector.offset = offset;
            wordsVector.maxsize = fetcher->max_size;
        }
    }
    tato_hyper_item_fetcher_free(fetcher);

    return wordsVector;
}

}; //end of namespace
