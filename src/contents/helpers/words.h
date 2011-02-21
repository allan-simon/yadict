#ifndef CONTENTS_HELPER_WORDS_H
#define CONTENTS_HELPER_WORDS_H

#include "models/Translations.h"
#include "models/Words.h"

#include "contents/Config.h"
#include "helpers.h"

namespace contents {

struct WordsHelper : public Helpers {
    results::WordsVector words;

    public:
        WordsHelper() {};
        WordsHelper(results::Word word) {
            if (word.exists()) {
                words.push_back(word);
            }
        }

        bool empty() {
            words.empty();
        };
};

}

#endif
