#ifndef SHDICT_GENERICS_LANGUAGES_H
#define SHDICT_GENERICS_LANGUAGES_H

#include <map>
#include "Singleton.h"

typedef std::map<std::string, std::string> ISOToNameMap;
typedef std::map<int, std::string> IdToISOMap;
typedef std::map<std::string, int> ISOToIdMap;

class Languages : public Singleton<Languages> {
    friend class Singleton<Languages>;

    private:
        ISOToNameMap isoToName;
        IdToISOMap idToISO;
        ISOToIdMap isoToId;
        Languages();

    public:
        ISOToNameMap get_iso_to_name_map();
        int get_id_from_iso(std::string isoCode);
        std::string get_iso_from_id(int id);

};

#endif
