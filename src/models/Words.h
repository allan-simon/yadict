#ifndef MODELS_WORDS_H
#define MODELS_WORDS_H

#include "models/TatoHyperDB.h"
#include <set>
#include <map>
extern "C" {
#include "tato/fetcher.h"
#include "tato/hyperitem.h"
}
#include "models/Logs.h"
#include "models/Metas.h"
#include "models/Meanings.h"

namespace results {
    struct Word;
}

typedef std::set<results::Word> WordsSet;
typedef std::map<std::string, WordsSet> TransMap;
typedef std::map<results::Meaning, TransMap> MeaningsTransMap;

namespace results {

    struct Word {
        int id;
        std::string text;
        std::string lang;
        int flags; //TODO replace this by tatohyperdb type
        MetasMap metas;
        MeaningsTransMap meanTransMap;
        TransMap transMap;

        public:
            Word(): id(0),text(""),lang(""),flags(0){};

            Word(int id, std::string text, std::string lang, int flags):
                id(id),
                text(text),
                lang(lang),
                flags(flags) {
            };

            Word(int id, char* text, char* lang, int flags):
                id(id),
                text(std::string(text)),
                lang(std::string(lang)),
                flags(flags) {
            };

            Word(TatoHyperItem* wordItem) {
                if (wordItem == NULL) {
                    id = 0;
                    return;
                }
                id = wordItem->id;
                text = std::string(wordItem->str);
                lang = std::string(wordItem->lang->code);
                flags = wordItem->flags;

            };

            bool exists() {
                return id > 0;
            }

    };

    struct WordsVector : public std::vector<Word> {
        int offset;
        int maxsize;
        public:
            WordsVector(): offset(0), maxsize(0) {};
            WordsVector(int size) : std::vector<Word>(size), offset(0), maxsize(0) {};
    };
}

namespace std {
    template<> struct less<results::Word> {
        bool operator() (const results::Word& lhs, const results::Word& rhs) {
            return lhs.id < rhs.id;
        }
    };

};


;



namespace models {

class Words {
    private:
        Logs logs;

    public:
        // TODO add a fonction to only retrieve a word, without meanings
        // nor translations metas etC.
        Words();
        results::Word get_word_with_id(int id);
        results::Word get_word_with_lang_str(
            std::string lang,
            std::string str
        );

        results::WordsVector get_words_with_str(std::string str);
        results::WordsVector get_words_with_str(
            std::string str,
            int size,
            int offset
        );

        results::WordsVector get_all_words();
        results::WordsVector get_all_words(int offset, int size);


        results::WordsVector get_all_words_in_lang(std::string lang);
        results::WordsVector get_all_words_in_lang(
            std::string lang,
            int size,
            int offset
        );

        int get_random_word_id();
        results::Word get_random_word();

        //TODO maybe replace bool by an fetcher if we want to
        //display the newly added word
        results::Word add_word(
            std::string lang,
            std::string str,
            int userId
        );
        results::Word add_word(
            std::string lang,
            std::string str,
            TatoHyperItemFlags flags,
            int userId
        );

        //TODO maybe replace bool by an fetcher if we want to
        //display the edited word
        bool edit_word(
            int id,
            std::string lang,
            std::string str,
            int userId
        );
        bool edit_word(
            int id,
            std::string lang,
            std::string str,
            TatoHyperItemFlags flags,
            int userId
        );

        //
        bool delete_word(int id, int userId);

        int get_translation_relation(int wordId);


        MeaningsTransMap pack_translations(
            TatoHyperItem* item,
            TransMap & packedTransWithoutMeaning
        );

        results::Word word_from_item(TatoHyperItem* item);

};

}

#endif
