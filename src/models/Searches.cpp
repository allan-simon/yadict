#include "models/Searches.h"
#include "models/Translations.h"

namespace models {

/**
 *
 */
Searches::Searches() {}



TatoHyperItemFetcher* Searches::simple(std::string query, std::string lang) {
    return simple(query, lang, 10, 0);
}


TatoHyperItemFetcher* Searches::simple(
    std::string query,
    std::string lang,
    int size,
    int offset
) {
    TatoHyperDb *tatoHyperDb = TatoHyperDB::getInstance("")->getDatabasePointer();
	TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(size, offset);

	TatoHyperItemLang *tatoLang = NULL;
	if (lang != "") {
		tatoLang = tato_hyper_db_lang_find(tatoHyperDb, lang.c_str());
        if (tatoLang != NULL) {
            tato_hyper_item_lang_items_suggest_pagi(
                tatoLang,
                fetcher,
                query.c_str()
            );
        }
    }
    return fetcher;
}
};
