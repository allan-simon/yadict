#ifndef MODELS_SEARCHES_H
#define MODELS_SEARCHES_H

#include <string>
#include <list>
#include <map>

extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
#include "tato/lang.h"
}
namespace models {

typedef std::list<TatoHyperItem*> TatoHyperItemPList;
typedef std::map<std::string, TatoHyperItemPList> TranslationsMap;

class Searches {
    public:
        Searches();

        TatoHyperItemFetcher* simple(std::string query, std::string lang);
        TatoHyperItemFetcher* simple(
            std::string query,
            std::string lang,
            int size,
            int offset
        );


};
};
#endif 

