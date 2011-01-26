#ifndef SHDICT_CONTENT_CONFIG_H
#define SHDICT_CONTENT_CONFIG_H

#include <map>
#include "generics/Singleton.h"


class Config : public Singleton<Config> {
    friend class Singleton<Config>;

    private:


        Config();

    public:
        std::string cssPath;
        std::string imgPath;
        std::string webPath;

};

#endif

