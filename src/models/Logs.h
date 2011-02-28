#ifndef MODELS_LOGS_H
#define MODELS_LOGS_H

#include "models/SqliteModel.h"
#include <vector>

#define ADD_ACTION    1 
#define DELETE_ACTION 2
#define EDIT_ACTION   3

using namespace cppdb;
namespace models {


struct WordsLogResult {
    int actionId;
    int eventType;
    std::string wordLang;
    std::string wordText;
    int userId;
    int timestamp;

};


struct MetasLogResult {
    int actionId;
    int eventType;
    
    std::string wordLang;
    int wordId;

    std::string key;
    std::string value;

    int userId;
    int timestamp;

};



typedef std::vector<WordsLogResult> WordsLogList;
typedef std::vector<MetasLogResult> MetasLogList;


//TODO divide this class in subclass (WordLog etc.)

class Logs : public SqliteModel {
    private:
        statement insertWordLog;
        statement getLastWordActions;

        statement insertMetaLog;
        statement getLastMetaActions;

        statement insertTransLog;

        statement insertMetaMeaningLog;

        statement insertMeaningLog;

        statement insertTransMeaningLog;
        //statement getLastTransActions;


        void insert_word_log(
            int wordId,
            std::string lang,
            std::string text,
            int userId,
            int eventType,
            std::string prevLang,
            std::string prevText
        );

        void insert_word_log(
            int wordId,
            std::string lang,
            std::string text,
            int userId,
            int eventType,
            std::string prevLang,
            std::string prevText,
            int actionId
        );
 
        void insert_meta_log(
            int wordId,
            std::string wordLang,

            std::string key,
            std::string value,
            int userId,
            int eventType,

            std::string prevValue
        );
               

        void insert_meta_log(
            int wordId,
            std::string wordLang,

            std::string key,
            std::string value,
            int userId,
            int eventType,

            std::string prevValue,
            bool isMain,
            int actionId
        );
        

        void insert_trans_log(
            int wordId,
            int wordId2,
            int eventType,
            int userId
        );

        void insert_trans_log(
            int wordId,
            int wordId2,
            int eventType,
            int userId,
            bool isMain,
            int actionId
        );

        void insert_trans_meaning_log(
            int meaningId,
            int wordId,
            int eventType,
            int userId
        );

        void insert_trans_meaning_log(
            int meaningId,
            int wordId,
            int eventType,
            int userId,
            bool isMain,
            int actionId
        );




        void insert_meaning_log(
            int meaningId,
            int wordId,
            int eventType,
            int userId
        );

        void insert_meaning_log(
            int meaningId,
            int wordId,
            int eventType,
            int userId,
            bool isMain,
            int actionId
        );

        void insert_meta_meaning_log(
            int meaningId,

            std::string key,
            std::string value,
            int userId,
            int eventType,

            std::string prevValue
        );


        void insert_meta_meaning_log(
            int meaningId,

            std::string key,
            std::string value,
            int userId,
            int eventType,

            std::string prevValue,
            bool isMain,
            int actionId
        );


    public:
        Logs(session sqliteDb);


        WordsLogList get_last_word_actions();
        
        void insert_add_word(
            int wordId,
            std::string lang,
            std::string text,
            int userId
        );
        
        void insert_edit_word(
            int wordId,
            std::string lang,
            std::string text,
            int userId,
            std::string prevLang,
            std::string prevText
        );
        
        void insert_delete_word(
            int wordId,
            std::string lang,
            std::string text,
            int userId
        );



        MetasLogList get_last_meta_actions();
        
        void insert_add_meta(
            int wordId,
            std::string wordLang,
            std::string key,
            std::string value,
            int userId
        );
        
        void insert_edit_meta(
            int wordId,
            std::string wordLang,
            std::string key,
            std::string value,
            int userId,
            std::string prevValue
        );
        
        void insert_delete_meta(
            int wordId,
            std::string wordLang,
            std::string key,
            std::string value,
            int userId,
            bool isMain,
            int actionId
        );



        void insert_add_trans(
            int wordId1,
            int wordId2,
            int userId
        );

        void insert_delete_trans(
            int wordId1,
            int wordId2,
            int userId,
            bool isMain,
            int actionId
        );


        //
        void insert_add_meta_meaning(
            int meaningId,
            std::string key,
            std::string value,
            int userId
        );
        
        void insert_edit_meta_meaning(
            int meaningId,
            std::string key,
            std::string value,
            int userId,
            std::string prevValue
        );
        

        void insert_delete_meta_meaning(
            int meaningId,
            std::string key,
            std::string value,
            int userId
        );

        void insert_delete_meta_meaning(
            int meaningId,
            std::string key,
            std::string value,
            int userId,
            bool isMain,
            int actionId
        );


        //
        void insert_add_meaning(
            int meaningId,
            int wordId,
            int userId
        );


        void insert_delete_meaning(
            int meaningId,
            int wordId,
            int userId
        );

        void insert_delete_meaning(
            int meaningId,
            int wordId,
            int userId,
            bool isMain,
            int actionId
        );
        //
        
        void insert_add_trans_meaning(
            int meaningId,
            int wordId,
            int userId
        );

        void insert_delete_trans_meaning(
            int meaningId,
            int wordId,
            int userId,
            bool isMain,
            int actionId
        );
        //


};

};

#endif
