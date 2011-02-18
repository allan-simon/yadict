#include "Languages.h"

Languages::Languages() {
    isoToName["wuu"] = "Shanghainese";
    isoToName["fra"] = "French";
    isoToName["eng"] = "English";
    isoToName["deu"] = "German";
    isoToName["cmn"] = "Mandarin";
    isoToName["yue"] = "Cantonese";


    idToISO[61] = "wuu";
    idToISO[23] = "fra";
    idToISO[17] = "eng";
    idToISO[15] = "deu";
    idToISO[12] = "cmn";
    idToISO[63] = "yue";
 
    isoToId["wuu"] = 61;
    isoToId["fra"] = 23;
    isoToId["eng"] = 17;
    isoToId["deu"] = 15;
    isoToId["cmn"] = 12;
    isoToId["yue"] = 63;



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
