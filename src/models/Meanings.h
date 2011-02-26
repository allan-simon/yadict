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

#include <map>

typedef std::map<std::string, std::string> DefsMap;
typedef std::map<std::string, std::string> MetasMap;
namespace results {
    struct Meaning {
        int id;
        // a map of lang => definition
        DefsMap defsMap;
        
        public:
            Meaning(): id(0) {};

            bool exists() {
                return id > 0;   
            };
    };

};

// otherwise we would not be able to use results::Meaning has std::map's key
namespace std {
    template<> struct less<results::Meaning> {
        bool operator() (const results::Meaning& lhs, const results::Meaning& rhs) {
            return lhs.id < rhs.id;
        }
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

        bool add_def_in(
            int meaningId,
            std::string definitionText,
            std::string definitionLang
        );


        bool edit_def_in(
            int meaningId,
            std::string definitionText,
            std::string definitionLang
        );

        bool delete_def_in(int meaningId, std::string defLang);
        bool delete_by_id(int meaningId); 
};

}
#endif

