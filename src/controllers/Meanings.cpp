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
    d->assign("/edit/(\\d+)/(\\w+)/(\\d+)", &Meanings::edit, this, 1, 2, 3);
    d->assign("/edit-treat", &Meanings::edit_treat, this);
    d->assign("/delete-by-id/(\\d+)", &Meanings::delete_by_id, this, 1);

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
    std::string redirectUrl = "/" + c.lang +"/words/show-all" ;
  
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
            defLang
        );

        go_back_to_previous_page();
        return;

    }
    //TODO handle if the ID of original word send in headers 
    //are not an id
    response().set_redirect_header(redirectUrl);
}

/**
 *
 */

void Meanings::edit(
    std::string meaningIdStr,
    std::string meaningLang,
    std::string origWordIdStr
) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int origWordId = atoi(origWordIdStr.c_str());
    int meaningId = atoi(meaningIdStr.c_str());
    
    contents::MeaningsEdit c;
    init_content(c);
 
    
    models::Words wordsModel;
	contents::WordsHelper whc(
        wordsModel.get_word_with_id(origWordId)
    );
    whc.lang = c.lang;

    results::Meaning meaning = meaningsModel.get_meaning_by_id(
        meaningId
    );

     // if no item with this id

    if (whc.empty() || meaning.id == 0) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        return;
    }
   

    c.editMeaning.definitionText.value(meaning.defsMap[meaningLang]);
    c.editMeaning.definitionLang.value(meaningLang);

    c.editMeaning.meaningId.value(meaningIdStr);
    c.editMeaning.origWordId.value(origWordIdStr);
    
    c.whc = whc;
    render("meanings_edit", c);   

}

void Meanings::edit_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();
 
    forms::EditMeaning formEdit;
    formEdit.load(context());

    if (!formEdit.validate()) {
        go_back_to_previous_page();
        return;
    }

    std::string defLang = formEdit.definitionLang.value();
    std::string defText = formEdit.definitionText.value();
    std::string meaningIdStr = formEdit.meaningId.value();
    std::string origWordIdStr = formEdit.origWordId.value();
    int meaningId = atoi(meaningIdStr.c_str());
    int origWordId = atoi(origWordIdStr.c_str());

   
    meaningsModel.edit(
        meaningId,
        defText,
        defLang
    );
   

    models::Words wordsModel;
    results::Word word = wordsModel.get_word_with_id(origWordId);

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

void Meanings::delete_by_id(std::string meaningIdStr) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int meaningId = atoi(meaningIdStr.c_str());

    // TODO handle error
    meaningsModel.delete_by_id(meaningId);

    go_back_to_previous_page();
}

};


