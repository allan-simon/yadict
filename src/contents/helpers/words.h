#ifndef CONTENTS_HELPER_WORDS_H
#define CONTENTS_HELPER_WORDS_H

#include <cppcms/view.h>
extern "C" {
    #include "tato/hyperitem.h"
    #include "tato/fetcher.h"
}

#include "models/Translations.h"
#include "models/Words.h"

namespace contents {

struct WordsHelper : public cppcms::base_content {
    TatoHyperItemFetcher *fetcher;
    models::TranslationsMap packedTrans;

    public:
        bool is_empty() {
            if (fetcher->items == NULL) {
                return NULL;
            }
            return fetcher->items[0] == NULL;
        }
};

}

#endif
