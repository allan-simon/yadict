#ifndef MODELS_WORDS_H
#define MODELS_WORDS_H

#include "models/TatoHyperDB.h"

extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
}

namespace models {

class Words {
    public:
        Words();
        TatoHyperItemFetcher* getWordWithId(int id);
        TatoHyperItemFetcher* getWordsWithStr(std::string str);
        TatoHyperItemFetcher* getWordsWithStr(
            std::string str,
            int size,
            int offset
        );

        TatoHyperItemFetcher* getAllWords();
        TatoHyperItemFetcher* getAllWords(int size, int offset);


        TatoHyperItemFetcher* getAllWordsInLang(std::string lang);
        TatoHyperItemFetcher* getAllWordsInLang(
            std::string lang,
            int size,
            int offset
        );

        int getRandomWordId();
        TatoHyperItemFetcher* getRandomWord();

        //TODO maybe replace bool by an fetcher if we want to
        //display the newly added word
        bool addWord(std::string lang, std::string str);
        bool addWord(
            std::string lang,
            std::string str,
            TatoHyperItemFlags flags
        );
};

}

#endif
