#ifndef MODELS_SEARCHES_H
#define MODELS_SEARCHES_H

#include <string>
#include <set>
#include <map>
#include <laputa.h>

#include "models/Words.h"


extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
#include "tato/lang.h"
}
namespace models {

typedef std::set<TatoHyperItem* > TatoHyperItemPList;
typedef std::map<std::string, TatoHyperItemPList> TranslationsMap;

class Searches {
    public:
        Searches();

        results::WordsVector simple(std::string query, std::string lang);
        results::WordsVector simple(
            std::string query,
            std::string lang,
            int size,
            int offset
        );


        results::WordsVector advance(std::string query, std::string lang);
        results::WordsVector advance(
            std::string query,
            std::string lang,
            int size,
            int offset
        );


};
};
#endif 

