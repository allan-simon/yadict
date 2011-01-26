#include "Languages.h"

Languages::Languages() {
    languagesMap["wuu"] = "Shanghainese";
    languagesMap["fra"] = "French";
    languagesMap["eng"] = "English";
    languagesMap["deu"] = "German";
    languagesMap["cmn"] = "Mandarin";
    languagesMap["yue"] = "Cantonese";



}

LangMap Languages::getMap() {
    return languagesMap;
}
