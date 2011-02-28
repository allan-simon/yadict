#include "Controller.h"
#include "controllers/Meanings.h"
#include "shanghainesedict.h"
#include "contents/meanings.h"

#include "models/TatoHyperDB.h"
#include "models/Meanings.h"

namespace controllers {

/**
 *
 */
Meanings::Meanings(cppcms::service &serv): Controller(serv) {
    cppcms::url_dispatcher* d = &dispatcher();

    d->assign("/add-to-word/(\\d+)", &Meanings::add_to_word, this, 1);
    d->assign("/add-to-word-treat", &Meanings::add_to_word_treat, this);
    d->assign("/show/(\\d+)/(\\d+)", &Meanings::show, this, 1, 2);
    d->assign("/edit-def-in/(\\d+)/(\\w+)/(\\d+)", &Meanings::edit_def_in, this, 1, 2, 3);
    d->assign("/edit-def-in-treat", &Meanings::edit_def_in_treat, this);
    d->assign("/add-def-in-treat", &Meanings::add_def_in_treat, this);
    d->assign("/delete-by-id/(\\d+)", &Meanings::delete_by_id, this, 1);
    d->assign("/delete-def-in/(\\d+)/(\\w+)", &Meanings::delete_def_in, this, 1, 2);

}

/**
 *
 */
void Meanings::add_to_word(std::string origWordId) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

	int origId = atoi(origWordId.c_str());
    contents::MeaningsAdd c;
    init_content(c);

    models::Words wordsModel;
    
	contents::WordsHelper whc(
        wordsModel.get_word_with_id(origId)
    );
    std::cout << "after model" << std::endl; 
    whc.lang = c.lang;


    if (whc.empty()) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        return;
    }

    c.addMeaning.origWordId.value(origWordId);
    
    c.whc = whc;
    std::cout << "before render" << std::endl; 
    render("meanings_add_to_word", c);   
}

/**
 *
 */

void Meanings::add_to_word_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();
    
    contents::MeaningsAdd c;
    init_content(c);
  
    models::Words wordsModel;

    c.addMeaning.load(context());

    if (c.addMeaning.validate()) {
        // TODO : handle if something wrong happen while saving
        std::string defLang = c.addMeaning.definitionLang.selected_id();
        std::string defText = c.addMeaning.definitionText.value();
        std::string strOrigWordId = c.addMeaning.origWordId.value();
        int origWordId = atoi(strOrigWordId.c_str());
    
        meaningsModel.add_to(
            origWordId,
            defText,
            defLang,
            get_current_user_id()
        );


    }
    go_back_to_previous_page();
    //TODO handle if the ID of original word send in headers 
    //are not an id
}

/**
 *
 */
void Meanings::edit_def_in(
    std::string meaningIdStr,
    std::string defLang,
    std::string wordIdStr
) {

    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int wordId = atoi(wordIdStr.c_str());
    int meaningId = atoi(meaningIdStr.c_str());
    
    contents::MeaningsEditDefIn c;
    init_content(c);
    c.defLang = defLang;
 
    
    models::Words wordsModel;
	contents::WordsHelper whc(
        wordsModel.get_word_with_id(wordId)
    );
    whc.lang = c.lang;

    c.wordId = wordId;
    c.meaning = meaningsModel.get_meaning_by_id(
        meaningId
    );

     // if no item with this id

    if (whc.empty() || c.meaning.id == 0) {
        go_back_to_previous_page();
        return;
    }

    c.editDefInMeaning.meaningId.value(meaningIdStr);
    c.editDefInMeaning.defLang.value(defLang);
    c.editDefInMeaning.wordId.value(wordIdStr);

    c.whc = whc;
    render("meanings_edit_def_in", c);   
}


void Meanings::show(
    std::string meaningIdStr,
    std::string wordIdStr
) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int wordId = atoi(wordIdStr.c_str());
    int meaningId = atoi(meaningIdStr.c_str());
    
    contents::MeaningsShow c;
    init_content(c);
 
    
    models::Words wordsModel;
	contents::WordsHelper whc(
        wordsModel.get_word_with_id(wordId)
    );
    whc.lang = c.lang;

    c.meaning = meaningsModel.get_meaning_by_id(
        meaningId
    );
    c.wordId = wordId; 


     // if no item with this id

    if (whc.empty() || c.meaning.id == 0) {
        go_back_to_previous_page();
        return;
    }

    c.addDefInMeaning.meaningId.value(meaningIdStr);

    c.whc = whc;
    render("meanings_show", c);   
}

/**
 *
 */

void Meanings::add_def_in_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();
    
    forms::AddDefInMeaning addDefInMeaning;
    models::Words wordsModel;

    addDefInMeaning.load(context());

    if (addDefInMeaning.validate()) {
        // TODO : handle if something wrong happen while saving
        std::string defLang = addDefInMeaning.defLang.selected_id();
        std::string defText =  addDefInMeaning.defText.value();
        std::string meaningIdStr = addDefInMeaning.meaningId.value();
        int meaningId = atoi(meaningIdStr.c_str());
    
        meaningsModel.add_def_in(
            meaningId,
            defText,
            defLang,
            get_current_user_id()
        );


    }
    go_back_to_previous_page();
    //TODO handle if the ID of original word send in headers 
    //are not an id
}





void Meanings::edit_def_in_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();
 
    forms::EditDefInMeaning formEdit;
    formEdit.load(context());

    if (!formEdit.validate()) {
        go_back_to_previous_page();
        return;
    }

    std::string defLang = formEdit.defLang.value();
    std::string defText = formEdit.defText.value();
    std::string meaningIdStr = formEdit.meaningId.value();
    std::string wordIdStr = formEdit.wordId.value();
    int meaningId = atoi(meaningIdStr.c_str());
    int wordId = atoi(wordIdStr.c_str());

   
    meaningsModel.edit_def_in(
        meaningId,
        defText,
        defLang,
        get_current_user_id()
    );
   

    models::Words wordsModel;
    results::Word word = wordsModel.get_word_with_id(wordId);

    if (word.exists()) {
        response().set_redirect_header(
        "/" + get_interface_lang() +"/words/show-in" +
        "/" + word.text +
        "/" + word.lang 
        );
    } else {
        go_back_to_previous_page();
    }
   
}


/**
 *
 */
void Meanings::delete_def_in(
    std::string meaningIdStr,
    std::string defLang
) {
    int meaningId = atoi(meaningIdStr.c_str());

    // TODO handle error
    meaningsModel.delete_def_in(
        meaningId,
        defLang ,
        get_current_user_id()   
    );

    //TODO redirect to word page
    response().set_redirect_header(
        "/" + get_interface_lang() +"/words/show-all"
    );

};


/**
 *
 */

void Meanings::delete_by_id(std::string meaningIdStr) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int meaningId = atoi(meaningIdStr.c_str());

    // TODO handle error
    meaningsModel.delete_by_id(
        meaningId,
        get_current_user_id()
    );

    go_back_to_previous_page();
}

};


