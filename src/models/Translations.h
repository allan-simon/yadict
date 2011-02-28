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
#include "models/Logs.h"
#include "models/Words.h"

#include <vector>
typedef std::vector<int> TransVector;

namespace models {

class Translations {
    private:
        Logs logs;
    public:
        Translations();

        bool add_translation_to_meaning(
            int meaningId,
            int fromWordId,
            std::string transText,
            std::string transLang,
            int userId
        );

        results::Word add_translation_to_word(
            int fromWordId,
            int transRelId,
            std::string transText,
            std::string transLang,
            int userId
        );

        bool add_one_way_link(
            int fromWordId,
            int transRelId,
            int toWordId
        );

        bool remove_from_words(
            int wordId1,
            int wordId2,
            int userId
        );

        bool remove_one_way(
            int transId,
            int origId
        );

        bool remove_from_meaning(
            int transId,
            int meaningId
        );

        TransVector get_all_trans_id(int wordId);
        TransVector get_all_trans_meaning(int meaningId);
};

}
#endif
