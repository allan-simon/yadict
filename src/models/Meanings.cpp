#include "models/Meanings.h"
#include "generics/Languages.h"

namespace models {
/**
 *
 */
Meanings::Meanings() :
    // TODO rather use a path in config
    logs(cppdb::session("sqlite3:db=../doc/sqlite3.db")) {

}

/**
 *
 */
bool Meanings::add_to(
    int wordId,
    std::string defText,
    std::string defLang,
    int userId
) { 
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 


    // if the word exist 
    // NOTE we trust what is send by the request 
    // so yep maybe the request is crafted , but i don't think there's any
    // security threat there
    if (wordId > 0) {

        TatoHyperRelation* meaning = tato_hyper_db_relation_add(
            tatoHyperDb,
            wordId,
            wordId,
            SHDICT_MEANING_REL_FLAG,
            0
        );

        if (meaning == NULL) {
            return false;
        }

        logs.insert_add_meaning(
            meaning->id,
            wordId,
            userId
        );

        if (!tato_hyper_relation_meta_set(
            meaning,
            defLang.c_str(),
            defText.c_str()
        )) {
            bool metaAdded = tato_hyper_relation_meta_add(
                meaning,
                tato_hyper_db_common_str(tatoHyperDb, defLang.c_str()),
                defText.c_str()
            );

            if (metaAdded) {
                logs.insert_add_meta_meaning(
                    meaning->id,
                    defLang,
                    defText,
                    userId
                );

            } else {
                return false;
            }
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

    if (relation == NULL) {
        return meaning; 
    }
 
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
bool Meanings::add_def_in(
    int meaningId,
    std::string defText,
    std::string defLang,
    int userId
) { 
    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 


    // if the meaning exist 
    // NOTE we trust what is send by the request 
    // so yep maybe the request is crafted , but i don't think there's any
    // security threat there
    TatoHyperRelation* meaning = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );

    if (meaning == NULL) {
        return false;
    }
    if (meaning->type != SHDICT_MEANING_REL_FLAG) {
        return false;
    }

    // if a definition already exists on this language
    // then we don't change anything (as it's supposed 
    // to be a new one
    if (tato_hyper_relation_meta_get(meaning, defLang.c_str()) != NULL) {
        return false;
    }


    if (!tato_hyper_relation_meta_set(
        meaning,
        defLang.c_str(),
        defText.c_str()
    )) {
        bool metaAdded = tato_hyper_relation_meta_add(
            meaning,
            tato_hyper_db_common_str(tatoHyperDb, defLang.c_str()),
            defText.c_str()
        );

        if (!metaAdded) {
            return false;
        }

        logs.insert_add_meta_meaning(
            meaning->id,
            defLang,
            defText,
            userId
        );
    }

    return true;
}



/**
 *
 */
bool Meanings::edit_def_in(
    int meaningId,
    std::string defText,
    std::string defLang,
    int userId
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
        const char* prevValueC = tato_hyper_relation_meta_get(
            relation,
            defLang.c_str()
        );
        if (prevValueC == NULL) {
            return false;
        }
        std::string prevValue(prevValueC);

        tato_hyper_relation_meta_set(
            relation,
            defLang.c_str(),
            defText.c_str()
        ); 

        logs.insert_edit_meta_meaning(
            relation->id,
            defLang,
            defText,
            userId,
            prevValue
        );

        return true;
    }

    return false;
}

bool Meanings::delete_def_in(
    int meaningId,
    std::string defLang,
    int userId
) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    // TODO factorize this
    TatoHyperRelation* relation = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );

    if (relation == NULL) {
        return false;
    }
    if (relation->type != SHDICT_MEANING_REL_FLAG) {
        return false;
    }
    const char* valueC = tato_hyper_relation_meta_get(
        relation,
        defLang.c_str()
    );

    if (valueC == NULL) {
        return false;
    }
    std::string value(valueC);


    bool metaDeleted = tato_hyper_relation_meta_delete(
        relation,
        defLang.c_str()
    );
    if (!metaDeleted) {
        return false;
    }

    logs.insert_delete_meta_meaning(
        relation->id,
        defLang,
        value,
        userId
    );
}

/**
 *
 */
bool Meanings::delete_by_id(
    int meaningId,
    int userId
) {

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperRelation* relation = tato_hyper_db_relation_find(
        tatoHyperDb,
        meaningId
    );
    if (relation == NULL) {
        return false;
    }
    int wordId = relation->start->id;
    
    // we check that we're really deleting a meaning relation
    // (as one can try random number and delete other relations,
    // as they're sharing the same ids list yet
    if (relation->type == SHDICT_MEANING_REL_FLAG) {
        bool relDeleted = tato_hyper_db_relation_delete(
            tatoHyperDb,
            meaningId
        );

        if (!relDeleted) {
            return false;
        }
        logs.insert_delete_meaning(
            meaningId,
            wordId,
            userId
        );
    }
    return false;
}

/**
 *
 */

MeaningsVector Meanings::get_all_meanings_on_word(int wordId) {
    MeaningsVector meanings;

    TatoHyperDb *tatoHyperDb = GET_DB_POINTER(); 
    TatoHyperItem *word= tato_hyper_db_item_find(tatoHyperDb, wordId);
    if (word == NULL) {
        return meanings;
    }

	TatoHyperRelationsNode *it;
	TATO_HYPER_RELATIONS_FOREACH(word->startofs, it) {
        if (it->relation->type == SHDICT_MEANING_REL_FLAG) {
            meanings.push_back(it->relation->id);
        }
    }
    return meanings;
}


};// end of namespace
