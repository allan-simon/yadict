#ifndef YADICT_MODELS_SEARCH_ENGINE_H 
#define YADICT_MODELS_SEARCH_ENGINE_H  

#include <iostream>
#include <map>

#include <cppcms/json.h>
#include <booster/thread.h>
#include <dystopia.h>

#include "generics/Singleton.h"

// TODO change the name, as we also use it to store the LANG_METANAME index
typedef std::map<std::string, TCIDB*> LangsDbsMap;
typedef std::vector<std::string> IndexedMetas;
typedef std::map<std::string, IndexedMetas> LangsIndexedMetasMap;

struct SearchResults : public std::vector<int> {
    int offset;
    int maxsize;
    public:
        SearchResults(): offset(0), maxsize(0) {};
        SearchResults(int size) :
            std::vector<int>(size),
            offset(0),
            maxsize(0)
            {};
};

class SearchEngine : public Singleton<SearchEngine>{
    friend class Singleton<SearchEngine>;

    private:
        int errorCode;
        LangsDbsMap langsDbs;
        LangsIndexedMetasMap langsIndexedMetas;

        void add_index(std::string indexName);

        SearchResults search_one_index(
            std::string request,
            std::string lang,
            int size,
            int offset
        );
        

        SearchEngine();
        ~SearchEngine();

    public:
        void init_indexed_metas( 
            cppcms::json::object langsIndexedMetasJson
        );

        void add_word(
            int wordId,
            std::string text,
            std::string lang
        );

        void add_meta(
            int wordId, 
            std::string key,
            std::string value,
            std::string lang
        );




        void edit_word(
            int wordId,
            std::string oldText,
            std::string newText,
            std::string oldLang,
            std::string newLang
        );

        void edit_meta(
            int wordId, 
            std::string key,
            std::string value,
            std::string lang
        );


        void edit_lang(
            int wordId,
            std::string oldText,
            std::string oldLang,
            std::string newLang
        );

        void remove_word(
            int wordId,
            std::string lang
        );

        void remove_meta(
            int wordId, 
            std::string key,
            std::string value,
            std::string lang
        );



        SearchResults search(
            std::string request,
            std::string lang,
            int size,
            int offset
        );
        

};

#endif


