#include "models/Meanings.h"
#include "generics/Languages.h"

namespace models {
/**
 *
 */
Meanings::Meanings() {}

/**
 *
 */
bool Meanings::add_to(
    int fromWordId,
    std::string definitionText,
    std::string definitionLang
) { 
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 


    // if the word exist 
    // NOTE we trust what is send by the request 
    // so yep maybe the request is crafted , but i don't think there's any
    // security threat there
    if (fromWordId > 0) {

        TatoHyperRelation* meaning = tato_hyper_db_relation_add(
            tatoHyperDb,
            fromWordId,
            fromWordId,
            SHDICT_MEANING_REL_FLAG,
            0
        );

        if (meaning == NULL) {
            return false;
        }

        if (!tato_hyper_relation_meta_set(
            meaning,
            definitionLang.c_str(),
            definitionText.c_str()
        )) {
            return tato_hyper_relation_meta_add(
                meaning,
                tato_hyper_db_common_str(tatoHyperDb, definitionLang.c_str()),
                definitionText.c_str()
            );
        }


    }
    return true;
}
/**
 *
 */
results::Meaning Meanings::get_meaning_by_id(int meaningId) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperRelation* relation = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );

    results::Meaning meaning;
    meaning.id = 0;
    
 
    if (relation->type == SHDICT_MEANING_REL_FLAG) {
        meaning.id = relation->id;
    
        ISOToNameMap isoToName = Languages::get_instance()->get_iso_to_name_map();
        ISOToNameMap::const_iterator end = isoToName.end();
        for(ISOToNameMap::const_iterator itr = isoToName.begin(); itr != end; ++itr){

            const char* meta = tato_hyper_relation_meta_get(
                relation,
                itr->first.c_str()
            );
            if (meta != NULL) {
                meaning.defsMap[itr->first] = std::string(meta); 
            }
        }

    }
    return meaning;
}


/**
 *
 */
bool Meanings::edit(
    int meaningId,
    std::string definitionText,
    std::string definitionLang
) {
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperRelation* relation = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );
    if (relation == NULL) {
        std::cout << "problem with relation" << std::endl;
        return false;
    }
    if (relation->type == SHDICT_MEANING_REL_FLAG) {
        tato_hyper_relation_meta_set(
            relation,
            definitionLang.c_str(),
            definitionText.c_str()
        ); 
        return true;
    }

    return false;
}

/**
 *
 */
bool Meanings::delete_by_id(int meaningId) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperRelation* relation = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );
    
    // we check that we're really deleting a meaning relation
    // (as one can try random number and delete other relations,
    // as they're sharing the same ids list yet
    if (relation->type == SHDICT_MEANING_REL_FLAG) {
        return tato_hyper_db_relation_delete(
            tatoHyperDb,
            meaningId
        );
    }
    return false;
}

};
