#ifndef SHDICT_MODELS_TRANSLATIONS_H
#define SHDICT_MODELS_TRANSLATIONS_H 

#include "models/TatoHyperDB.h"

extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
#include "tato/hyperrelation.h"
#include "tato/hyperrelations.h"
}

#define SHDICT_TRANSLATION_REL_FLAG 0b1

namespace models {

class Translations {
    public:
        Translations();
        bool add_to(
            int fromWordId,
            int translationRelId,
            int toWordId
        );
};

}
#endif
