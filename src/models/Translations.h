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
        bool add_to(
            int fromWordId,
            int translationRelId,
            int toWordId
        );

        bool remove(
            int transId,
            int origId
        );
};

}
#endif
