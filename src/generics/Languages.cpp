#include "Languages.h"

Languages::Languages() {}

void Languages::init(
    cppcms::json::object langsJson
) {
    cppcms::json::object::const_iterator end = langsJson.end();

    for (
        cppcms::json::object::const_iterator itr = langsJson.begin();
        itr != end;
        ++itr
    ) { 
        cppcms::json::array lang = itr->second.array();
    
        isoToName[itr->first] = lang[0].str(); 
        int langId = (int) lang[1].number();

        isoToId[itr->first] = langId; 
        idToISO[langId] = itr->first;

    }
}


/**
 *
 */
ISOToNameMap Languages::get_iso_to_name_map() {
    return isoToName;
}

/**
 *
 */
int Languages::get_id_from_iso(std::string isoCode) {
    // TODO handle if the isoCode is a wrong one
    return isoToId[isoCode];
}


/**
 *
 */
std::string Languages::get_iso_from_id(int id) {
    // TODO handle if the isoCode is a wrong one
    return idToISO[id];
}
