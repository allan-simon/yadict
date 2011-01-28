#ifndef MODELS_METAS_H
#define MODELS_METAS_H

#include "models/TatoHyperDB.h"
#include <list>
#include <map>
extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
}

namespace models {


class Metas {
    public:
        Metas();
        bool add_meta(
            int wordId,
            std::string key,
            std::string value
        );
};

}

#endif

