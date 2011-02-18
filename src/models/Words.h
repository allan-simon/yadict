#ifndef MODELS_WORDS_H
#define MODELS_WORDS_H

#include "models/TatoHyperDB.h"
#include <set>
#include <map>
extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
}
#include "models/Logs.h"

namespace models {

typedef std::set<TatoHyperItem*> TatoHyperItemPList;
typedef std::map<std::string, TatoHyperItemPList> TranslationsMap;
typedef std::map<TatoHyperRelation* , TranslationsMap> MeaningsTranslationsMap;

class Words {
    private:
        Logs logs;

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
            std::string str,
            int userId
        );
        TatoHyperItem* add_word(
            std::string lang,
            std::string str,
            TatoHyperItemFlags flags,
            int userId
        );

        //TODO maybe replace bool by an fetcher if we want to
        //display the edited word
        bool edit_word(
            int id,
            std::string lang,
            std::string str,
            int userId
        );
        bool edit_word(
            int id,
            std::string lang,
            std::string str,
            TatoHyperItemFlags flags,
            int userId
        );

        //
        bool delete_word(int id, int userId);

        int get_translation_relation(TatoHyperItem* word);


        MeaningsTranslationsMap pack_translations(
            TatoHyperItemFetcher* fetcher,
            TranslationsMap & packedTransWithoutMeaning
        );

};

}

#endif
