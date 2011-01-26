#ifndef SHDICT_GENERICS_LANGUAGES_H
#define SHDICT_GENERICS_LANGUAGES_H

#include <map>
#include "Singleton.h"

typedef std::map<std::string, std::string> LangMap;

class Languages : public Singleton<Languages> {
    friend class Singleton<Languages>;

    private:
        LangMap languagesMap;
        Languages();

    public:
        LangMap getMap();

};

#endif
