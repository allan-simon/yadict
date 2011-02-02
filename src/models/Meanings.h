#ifndef SHDICT_MODELS_MEANINGS_H
#define SHDICT_MODELS_MEANINGS_H 

#include "models/TatoHyperDB.h"
#include "models/Relations.h"

extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
#include "tato/hyperrelation.h"
#include "tato/hyperrelations.h"
}
namespace results {
    struct Meaning {
        int id;
        // TODO replace this by a map  lang => definition
        std::string definition;
    };

};

namespace models {

class Meanings {
    public:
        Meanings();
        bool add_to(
            int fromWordId,
            std::string definitionText,
            std::string definitionLang
        );

        results::Meaning get_meaning_by_id(
            int meaningId
        );

        bool edit(
            int meaningId,
            std::string definitionText,
            std::string definitionLang
        );

        bool delete_by_id(int meaningId); 
};

}
#endif

