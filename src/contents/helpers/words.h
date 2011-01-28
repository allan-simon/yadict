#ifndef CONTENTS_HELPER_WORDS_H
#define CONTENTS_HELPER_WORDS_H

extern "C" {
    #include "tato/hyperitem.h"
    #include "tato/fetcher.h"
}

#include "models/Translations.h"
#include "models/Words.h"

#include "contents/Config.h"
#include "helpers.h"

namespace contents {

struct WordsHelper : public Helpers {
    TatoHyperItemFetcher *fetcher ;
    models::TranslationsMap packedTrans;

    public:
        WordsHelper() {
            fetcher = NULL;
        }

        bool is_empty() {
            if (fetcher == NULL) {
                return false;
                if (fetcher->items == NULL) {
                    return false;
                }
            }
            return fetcher->items[0] == NULL;
        }
};

}

#endif
