#include <cppdb/frontend.h>
#include "models/Logs.h"
#include "models/Metas.h"
#include "models/Meanings.h"
#include "models/Translations.h"
#include "generics/Languages.h"
#include "generics/ActionId.h"


/**
 *
 */
using namespace cppdb;

namespace models {

Logs::Logs(session sqliteDb) : SqliteModel(sqliteDb) {

    insertWordLog = sqliteDb.create_prepared_statement(
        "INSERT INTO words_logs ("
        "   word_id,"
        "   lang,"
        "   text,"

        "   action_id,"
        "   user_id,"
        "   event_type,"
        "   prev_lang,"
        "   prev_text,"
        "   event_date"
        ") "
        "VALUES (?,?,?, ?,?,?,?,?,strftime('%s','now'));"
    );

    getLastWordActions = sqliteDb.create_prepared_statement(
        "SELECT action_id, event_type, word_id, lang, text, user_id, event_date "
        "FROM words_logs LIMIT 100;"
    );

    //

    insertMetaLog = sqliteDb.create_prepared_statement(
        "INSERT INTO metas_logs ("
        "   is_main,"
        "   word_id,"
        "   word_lang,"

        "   meta_key,"
        "   meta_value,"

        "   action_id,"
        "   user_id,"
        "   event_type,"

        "   prev_key,"
        "   prev_value,"
        
        "   event_date"
        ") "
        "VALUES (?,?,? ,?,?, ?,?,?, ?,?, strftime('%s','now'));"
    );

    getLastMetaActions = sqliteDb.create_prepared_statement(
        "SELECT"
        "    action_id,"
        "    event_type,"
        "    word_id,"
        "    word_lang,"
        "    meta_key,"
        "    meta_value,"
        "    user_id,"
        "    event_date "
        "FROM metas_logs WHERE is_main = 1 "
        "LIMIT 100;"
    );


    //
    insertTransLog = sqliteDb.create_prepared_statement(
        "INSERT INTO trans_logs ("
        "   is_main,"
        "   word1_id,"
        "   word2_id,"

        "   action_id,"
        "   user_id,"
        "   event_type,"
        "   event_date"
        ") "
        "VALUES (?,?,?, ?,?,?,strftime('%s','now'));"
    );

    //
    insertMeaningLog = sqliteDb.create_prepared_statement(
        "INSERT INTO meanings_logs ("
        "   is_main,"
        "   meaning_id,"
        "   word_id,"

        "   action_id,"
        "   user_id,"
        "   event_type,"
        "   event_date"
        ") "
        "VALUES (?,?,?, ?,?,?,strftime('%s','now'));"
    );



    //
    insertTransMeaningLog = sqliteDb.create_prepared_statement(
        "INSERT INTO meanings_trans_logs ("
        "   is_main,"
        "   meaning_id,"
        "   word_id,"

        "   action_id,"
        "   user_id,"
        "   event_type,"
        "   event_date"
        ") "
        "VALUES (?,?,?, ?,?,?,strftime('%s','now'));"
    );


    //
    insertMetaMeaningLog = sqliteDb.create_prepared_statement(
        "INSERT INTO metas_meaning_logs ("
        "   is_main,"
        "   meaning_id,"

        "   meta_key,"
        "   meta_value,"

        "   action_id,"
        "   user_id,"
        "   event_type,"

        "   prev_key,"
        "   prev_value,"
        
        "   event_date"
        ") "
        "VALUES (?,? ,?,?, ?,?,?, ?,?, strftime('%s','now'));"
    );



}

/**
 *
 */

WordsLogList Logs::get_last_word_actions() {
    result res = getLastWordActions.query();
    WordsLogList wordsLog;

    while (res.next()) {
        WordsLogResult temp;
        
        temp.actionId = res.get<int>("action_id"); 
        temp.eventType = res.get<int>("event_type"); 
        temp.wordLang = Languages::get_instance()->get_iso_from_id(
            res.get<int>("lang")
        );
        temp.wordText = res.get<std::string>("text");
        temp.userId = res.get<int>("user_id");
        temp.timestamp = res.get<int>("event_date");
         
        wordsLog.push_back(temp);
    }
    getLastWordActions.reset();
    
    return wordsLog;
}


/**
 *
 */
void Logs::insert_add_word(
    int wordId,
    std::string lang,
    std::string text,
    int userId
) {
    insert_word_log(
        wordId,
        lang,
        text,
        userId,
        ADD_ACTION,
        "",
        ""
    );
}

/**
 *
 */
void Logs::insert_edit_word(
    int wordId,
    std::string lang,
    std::string text,
    int userId,
    std::string prevLang,
    std::string prevText
) {
    insert_word_log(
        wordId,
        lang,
        text,
        userId,
        EDIT_ACTION,
        prevLang,
        prevText
    );
}

/**
 *
 */
void Logs::insert_delete_word(
    int wordId,
    std::string lang,
    std::string text,
    int userId
) {
    
    int actionId = singletons::ActionId::get_instance()->get_action_id();


    // we first log the deletion of all the meta
    models::Metas metasModel;
    MetasMap metasMap = metasModel.get_all_metas_of_word(wordId);

    MetasMap::const_iterator end = metasMap.end();
    for (MetasMap::const_iterator it = metasMap.begin(); it != end; ++it) {
        insert_delete_meta(
            wordId,
            lang,
            it->first,
            it->second,
            userId,
            false,
            actionId
        );

    }


    // then deletion of all the meanings
    models::Meanings meaningsModel;
    MeaningsVector meanings = meaningsModel.get_all_meanings_on_word(wordId);

    MeaningsVector::const_iterator endMeanings = meanings.end();
    for (MeaningsVector::const_iterator it = meanings.begin(); it != endMeanings; ++it) {
        insert_delete_meaning(
            *it,
            wordId,
            userId,
            false,
            actionId
        );
    }



    // then deletion of all the translations link
    models::Translations transModel;
    TransVector transVector = transModel.get_all_trans_id(wordId);

    TransVector::const_iterator endTrans = transVector.end();
    for (TransVector::const_iterator it = transVector.begin(); it != endTrans; ++it) {
        insert_delete_trans(
            wordId,
            *it,
            userId,
            false,
            actionId
        );
    }

    // finally log deletion of word itself
    insert_word_log(
        wordId,
        lang,
        text,
        userId,
        DELETE_ACTION,
        "",
        "",
        actionId
    );
}




/**
 *
 */

void Logs::insert_word_log(
    int wordId,
    std::string lang,
    std::string text,
    int userId,
    int eventType,
    std::string prevLang,
    std::string prevText
) {

    int actionId = singletons::ActionId::get_instance()->get_action_id();
    insert_word_log (
        wordId,
        lang,
        text,
        userId,
        eventType,
        prevLang,
        prevText,
        actionId
    );

}

void Logs::insert_word_log(
    int wordId,
    std::string lang,
    std::string text,
    int userId,
    int eventType,
    std::string prevLang,
    std::string prevText,
    int actionId
) {


    insertWordLog.bind(wordId);
    insertWordLog.bind(
        Languages::get_instance()->get_id_from_iso(lang)
    );
    insertWordLog.bind(text);

    insertWordLog.bind(actionId);
    insertWordLog.bind(userId);
    insertWordLog.bind(eventType);

    if (prevLang.empty() && prevText.empty()) {
        insertWordLog.bind_null();
        insertWordLog.bind_null();
    } else {
        insertWordLog.bind(
            Languages::get_instance()->get_id_from_iso(lang)
        );
        insertWordLog.bind(prevText);
    }

    try {
        insertWordLog.exec();    
        insertWordLog.reset();
    } catch (cppdb::cppdb_error const &e) {

        std::cerr << "error occured while inserting add word to logs"
            << std::endl;
    }


}







/**
 *
 */

MetasLogList Logs::get_last_meta_actions() {
    result res = getLastMetaActions.query();
    MetasLogList metasLog;

    while (res.next()) {
        MetasLogResult temp;
        
        temp.actionId = res.get<int>("action_id"); 
        temp.eventType = res.get<int>("event_type"); 
        
        temp.wordLang = Languages::get_instance()->get_iso_from_id(
            res.get<int>("word_lang")
        );
        temp.wordId = res.get<int>("word_id");

        temp.key = res.get<std::string>("meta_key");
        temp.value = res.get<std::string>("meta_value");

        temp.userId = res.get<int>("user_id");
        temp.timestamp = res.get<int>("event_date");
         
        metasLog.push_back(temp);
    }
    getLastMetaActions.reset();
    
    return metasLog;
}


/**
 *
 */
void Logs::insert_add_meta(
    int wordId,
    std::string wordLang,
    std::string key,
    std::string value,
    int userId
) {
    insert_meta_log(
        wordId,
        wordLang,
        key,
        value,
        userId,
        ADD_ACTION,
        "",
        true,
        0
    );
}

/**
 *
 */
void Logs::insert_edit_meta(
    int wordId,
    std::string wordLang,
    std::string key,
    std::string value,
    int userId,
    std::string prevValue
) {
    insert_meta_log(
        wordId,
        wordLang,
        key,
        value,
        userId,
        EDIT_ACTION,
        prevValue,
        true,
        0
    );
}

/**
 *
 */
void Logs::insert_delete_meta(
    int wordId,
    std::string wordLang,
    std::string key,
    std::string value,
    int userId,
    bool isMain,
    int actionId
) {
    insert_meta_log(
        wordId,
        wordLang,
        key,
        value,
        userId,
        DELETE_ACTION,
        "",
        isMain,
        actionId
    );
}




/**
 *
 */
void Logs::insert_meta_log(
    int wordId,
    std::string wordLang,
    std::string key,
    std::string value,
    int userId,
    int eventType,
    std::string prevValue,
    bool isMain,
    int actionId
) {

    insertMetaLog.bind(isMain);
    insertMetaLog.bind(wordId);
    insertMetaLog.bind(
        Languages::get_instance()->get_id_from_iso(wordLang)
    );
    insertMetaLog.bind(key);
    insertMetaLog.bind(value);

    if (actionId == 0) {
        actionId = singletons::ActionId::get_instance()->get_action_id();
    }
    insertMetaLog.bind(actionId);
    insertMetaLog.bind(userId);
    insertMetaLog.bind(eventType);

    if (prevValue.empty()) {
        insertMetaLog.bind_null();
    } else {
        insertMetaLog.bind(prevValue);
    }

    try {
        insertMetaLog.exec();    
        insertMetaLog.reset();
    } catch (cppdb::cppdb_error const &e) {

        std::cerr << "error occured while inserting add word to logs"
            << std::endl;
    }


}


/**
 *
 */
void Logs::insert_add_trans(
    int wordId1,
    int wordId2,
    int userId
) {
    insert_trans_log(
        wordId1,
        wordId2,
        ADD_ACTION,
        userId
    );
    
}


/**
 *
 */
void Logs::insert_delete_trans(
    int wordId1,
    int wordId2,
    int userId,
    bool isMain,
    int actionId
) {
    insert_trans_log(
        wordId1,
        wordId2,
        DELETE_ACTION,
        userId,
        isMain,
        actionId
    );

}

/**
 *
 */

void Logs::insert_trans_log(
    int wordId,
    int wordId2,
    int eventType,
    int userId
) {
    insert_trans_log(
        wordId,
        wordId2,
        eventType,
        userId,
        true,
        0
    );

}

void Logs::insert_trans_log(
    int wordId,
    int wordId2,
    int eventType,
    int userId,
    bool isMain,
    int actionId
) {

    insertTransLog.bind(isMain);
    insertTransLog.bind(wordId);
    insertTransLog.bind(wordId2);

    if (actionId == 0) {
        actionId = singletons::ActionId::get_instance()->get_action_id();
    }
    insertTransLog.bind(actionId);
    insertTransLog.bind(userId);
    insertTransLog.bind(eventType);

    try {
        insertTransLog.exec();    
        insertTransLog.reset();
    } catch (cppdb::cppdb_error const &e) {

        std::cerr << "error occured while inserting add word to logs"
            << std::endl;
    }



}

//
void Logs::insert_add_meta_meaning(
    int meaningId,
    std::string key,
    std::string value,
    int userId
) {

    insert_meta_meaning_log(
        meaningId,
        key,
        value,
        userId,
        ADD_ACTION,
        ""
    );

}

void Logs::insert_edit_meta_meaning(
    int meaningId,
    std::string key,
    std::string value,
    int userId,
    std::string prevValue
) {

    insert_meta_meaning_log(
        meaningId,
        key,
        value,
        userId,
        EDIT_ACTION,
        prevValue
    );


}

void Logs::insert_delete_meta_meaning(
    int meaningId,
    std::string key,
    std::string value,
    int userId
) {

    int actionId = singletons::ActionId::get_instance()->get_action_id();
    insert_meta_meaning_log(
        meaningId,
        key,
        value,
        userId,
        DELETE_ACTION,
        "",
        true,
        actionId
    );
}


void Logs::insert_delete_meta_meaning(
    int meaningId,
    std::string key,
    std::string value,
    int userId,
    bool isMain,
    int actionId
) {

    insert_meta_meaning_log(
        meaningId,
        key,
        value,
        userId,
        DELETE_ACTION,
        "",
        isMain,
        actionId
    );


}

void Logs::insert_meta_meaning_log(
    int meaningId,

    std::string key,
    std::string value,
    int userId,
    int eventType,

    std::string prevValue
) {
    int actionId = singletons::ActionId::get_instance()->get_action_id();

    insert_meta_meaning_log(
        meaningId,

        key,
        value,
        userId,
        eventType,
        prevValue,
        true,
        actionId
    );
}

void Logs::insert_meta_meaning_log(
    int meaningId,

    std::string key,
    std::string value,
    int userId,
    int eventType,

    std::string prevValue,
    bool isMain,
    int actionId
) {

    insertMetaMeaningLog.bind(isMain);
    insertMetaMeaningLog.bind(meaningId);
    insertMetaMeaningLog.bind(key);
    insertMetaMeaningLog.bind(value);

    insertMetaMeaningLog.bind(actionId);
    insertMetaMeaningLog.bind(userId);
    insertMetaMeaningLog.bind(eventType);

    if (prevValue.empty()) {
        insertMetaMeaningLog.bind_null();
    } else {
        insertMetaMeaningLog.bind(prevValue);
    }

    try {
        insertMetaMeaningLog.exec();    
        insertMetaMeaningLog.reset();
    } catch (cppdb::cppdb_error const &e) {

        std::cerr << "error occured while inserting add word to logs"
            << std::endl;
    }



}


/**
 *
 */
void Logs::insert_add_meaning(
    int meaningId,
    int wordId,
    int userId
) {
    int actionId = singletons::ActionId::get_instance()->get_action_id();

    insert_meaning_log(
        meaningId,
        wordId,
        ADD_ACTION,
        userId,
        true,
        0
    );

}

void Logs::insert_delete_meaning(
    int meaningId,
    int wordId,
    int userId
) {
    int actionId = singletons::ActionId::get_instance()->get_action_id();
    models::Meanings meaningsModel;

    results::Meaning meaning = meaningsModel.get_meaning_by_id(meaningId);
    if (!meaning.exists()) {
        return;
    }
    // log deletion of all the metas that were set on this meaning
    DefsMap::const_iterator end  = meaning.defsMap.end(); 
    for (
        DefsMap::const_iterator itr = meaning.defsMap.begin();
        itr != end;
        ++itr
    ) {
        insert_delete_meta_meaning(
            meaningId,
            itr->first,
            itr->second,
            userId,
            false,
            actionId
        );

    }
    // log deletion of all 'translation' links that were set on this meaning
    models::Translations transModel;
    TransVector translations = transModel.get_all_trans_meaning(meaningId);
 
    TransVector::const_iterator endTrans = translations.end();
     for (TransVector::const_iterator it = translations.begin(); it != endTrans; ++it) {
        insert_delete_trans_meaning(
            meaningId,
            *it,
            userId,
            false,
            actionId
        );
    }

    // now we log the deletion of the meaning itself
    insert_meaning_log(
        meaningId,
        wordId,
        DELETE_ACTION,
        userId,
        true,
        actionId
    );

}


void Logs::insert_delete_meaning(
    int meaningId,
    int wordId,
    int userId,
    bool isMain,
    int actionId
) {
    // TODO factorize
    models::Meanings meaningsModel;

    results::Meaning meaning = meaningsModel.get_meaning_by_id(meaningId);
    if (!meaning.exists()) {
        return;
    }
    // log deletion of all the metas that were set on this meaning
    DefsMap::const_iterator end  = meaning.defsMap.end(); 
    for (
        DefsMap::const_iterator itr = meaning.defsMap.begin();
        itr != end;
        ++itr
    ) {
        insert_delete_meta_meaning(
            meaningId,
            itr->first,
            itr->second,
            userId,
            false,
            actionId
        );

    }
    // log deletion of all 'translation' links that were set on this meaning
    models::Translations transModel;
    TransVector translations = transModel.get_all_trans_meaning(meaningId);
 
    TransVector::const_iterator endTrans = translations.end();
     for (TransVector::const_iterator it = translations.begin(); it != endTrans; ++it) {
        insert_delete_trans_meaning(
            meaningId,
            *it,
            userId,
            false,
            actionId
        );
    }


    insert_meaning_log(
        meaningId,
        wordId,
        DELETE_ACTION,
        userId,
        isMain,
        actionId
    );

}

void Logs::insert_meaning_log(
    int meaningId,
    int wordId,
    int eventType,
    int userId
) {

    int actionId = singletons::ActionId::get_instance()->get_action_id();
    insert_meaning_log(
        meaningId,
        wordId,
        eventType,
        userId,
        true,
        actionId
    );
}

void Logs::insert_meaning_log(
    int meaningId,
    int wordId,
    int eventType,
    int userId,
    bool isMain,
    int actionId
) {
    insertMeaningLog.bind(isMain);
    insertMeaningLog.bind(meaningId);
    insertMeaningLog.bind(wordId);

    insertMeaningLog.bind(actionId);
    insertMeaningLog.bind(userId);
    insertMeaningLog.bind(eventType);

    try {
        insertMeaningLog.exec();    
        insertMeaningLog.reset();
    } catch (cppdb::cppdb_error const &e) {

        std::cerr << "error occured while in trans_meaning_logs"
            << std::endl;
    }


}




//

/**
 *
 */
void Logs::insert_add_trans_meaning(
    int meaningId,
    int wordId,
    int userId
) {
    insert_trans_meaning_log(
        meaningId,
        wordId,
        ADD_ACTION,
        userId,
        true,
        0
    );

}

void Logs::insert_delete_trans_meaning(
    int meaningId,
    int wordId,
    int userId,
    bool isMain,
    int actionId
) {
    insert_trans_meaning_log(
        meaningId,
        wordId,
        DELETE_ACTION,
        userId,
        isMain,
        actionId
    );


}

void Logs::insert_trans_meaning_log(
    int meaningId,
    int wordId,
    int eventType,
    int userId
) {

    int actionId = singletons::ActionId::get_instance()->get_action_id();
    insert_trans_meaning_log(
        meaningId,
        wordId,
        eventType,
        userId,
        true,
        actionId
    );
}

void Logs::insert_trans_meaning_log(
    int meaningId,
    int wordId,
    int eventType,
    int userId,
    bool isMain,
    int actionId
) {
    insertTransMeaningLog.bind(isMain);
    insertTransMeaningLog.bind(meaningId);
    insertTransMeaningLog.bind(wordId);

    insertTransMeaningLog.bind(actionId);
    insertTransMeaningLog.bind(userId);
    insertTransMeaningLog.bind(eventType);

    try {
        insertTransMeaningLog.exec();    
        insertTransMeaningLog.reset();
    } catch (cppdb::cppdb_error const &e) {

        std::cerr << "error occured while in trans_meaning_logs"
            << std::endl;
    }


}


}; // end namespace models


