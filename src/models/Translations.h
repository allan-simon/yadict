#ifndef SHDICT_MODELS_TRANSLATIONS_H
#define SHDICT_MODELS_TRANSLATIONS_H 

#include "models/TatoHyperDB.h"
#include "models/Relations.h"

extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
#include "tato/hyperrelation.h"
#include "tato/hyperrelations.h"
}


namespace models {

class Translations {
    public:
        Translations();

        bool add_translation_to_meaning(
            int meaningId,
            int fromWordId,
            std::string transText,
            std::string transLang
        );

        TatoHyperItem* add_translation_to_word(
            int fromWordId,
            int transRelId,
            std::string transText,
            std::string transLang
        );

        bool add_one_way_link(
            int fromWordId,
            int transRelId,
            int toWordId
        );

        bool remove(
            int transId,
            int origId
        );

        bool remove_from_meaning(
            int transId,
            int meaningId
        );
};

}
#endif
