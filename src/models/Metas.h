#ifndef MODELS_METAS_H
#define MODELS_METAS_H

#include "models/TatoHyperDB.h"
#include <list>
#include <map>
extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
}
#include "models/Logs.h"

typedef std::map<std::string, std::string> MetasMap;

namespace models {


class Metas {
    private:
        Logs logs;

    public:
        Metas();
        bool add_meta(
            int wordId,
            std::string key,
            std::string value,
            int userId
        );

        bool edit_meta(
            int wordId,
            std::string key,
            std::string newValue,
            int userId
        );

        bool remove_meta(
            int wordId,
            std::string key,
            int userId
        );

        bool has_meta(
            int wordId,
            std::string key
        );

        std::string get_meta_from_key(
            int wordId,
            std::string key
        );

        MetasMap get_all_metas_of_word(
            int wordId
        );
};

}

#endif

