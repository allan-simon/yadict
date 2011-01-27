#ifndef MODELS_WORDS_H
#define MODELS_WORDS_H

#include "models/TatoHyperDB.h"
#include <list>
#include <map>
extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
}

namespace models {

typedef std::list<TatoHyperItem*> TatoHyperItemPList;
typedef std::map<std::string, TatoHyperItemPList> TranslationsMap;

class Words {
    public:
        Words();
        TatoHyperItemFetcher* get_word_with_id(int id);
        TatoHyperItem* get_word_with_lang_str(
            std::string lang,
            std::string str
        );
        TatoHyperItemFetcher* get_words_with_str(std::string str);
        TatoHyperItemFetcher* get_words_with_str(
            std::string str,
            int size,
            int offset
        );

        TatoHyperItemFetcher* get_all_words();
        TatoHyperItemFetcher* get_all_words(int offset, int size);


        TatoHyperItemFetcher* get_all_words_in_lang(std::string lang);
        TatoHyperItemFetcher* get_all_words_in_lang(
            std::string lang,
            int size,
            int offset
        );

        int get_random_word_id();
        TatoHyperItemFetcher* get_random_word();

        //TODO maybe replace bool by an fetcher if we want to
        //display the newly added word
        TatoHyperItem* add_word(
            std::string lang,
            std::string str
        );
        TatoHyperItem* add_word(
            std::string lang,
            std::string str,
            TatoHyperItemFlags flags
        );

        //TODO maybe replace bool by an fetcher if we want to
        //display the edited word
        bool edit_word(
            int id,
            std::string lang,
            std::string str
        );
        bool edit_word(
            int id,
            std::string lang,
            std::string str,
            TatoHyperItemFlags flags
        );

        //
        bool delete_word(int id);

        int get_translation_relation(TatoHyperItem* word);


        TranslationsMap pack_translations(
            TatoHyperItemFetcher* fetcher
        );

};

}

#endif
