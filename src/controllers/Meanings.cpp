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
    d->assign("/edit/(\\d+)/(\\d+)", &Meanings::edit, this, 1, 2);
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

	contents::WordsHelper whc;
    models::Words wordsModel;
    whc.lang = c.lang;
    
    whc.fetcher = wordsModel.get_word_with_id(origId);
    whc.packedTrans = wordsModel.pack_translations(whc.fetcher); 

    TatoHyperItem* word = whc.fetcher->items[0];
     // if no item with this id

    if (word == NULL) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        tato_hyper_item_fetcher_free(whc.fetcher);
        return;
    }

    c.addMeaning.origWordId.value(origWordId);
    
    c.whc = whc;
    render("meanings_add_to_word", c);   

    tato_hyper_item_fetcher_free(whc.fetcher);
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

void Meanings::edit(std::string meaningIdStr, std::string origWordIdStr) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int origWordId = atoi(origWordIdStr.c_str());
    int meaningId = atoi(meaningIdStr.c_str());
    
    contents::MeaningsEdit c;
    init_content(c);
 
	contents::WordsHelper whc;
    whc.lang = c.lang;
    
    models::Words wordsModel;
    whc.fetcher = wordsModel.get_word_with_id(origWordId);
    TatoHyperItem *word = whc.fetcher->items[0];

    results::Meaning meaning = meaningsModel.get_meaning_by_id(
        meaningId
    );

     // if no item with this id

    if (word == NULL || meaning.id == 0) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        tato_hyper_item_fetcher_free(whc.fetcher);
        return;
    }
   

    c.editMeaning.definitionText.value(meaning.definition);
    c.editMeaning.meaningId.value(meaningIdStr);
    c.editMeaning.origWordId.value(origWordIdStr);
    
    c.whc = whc;
    render("meanings_edit", c);   

    tato_hyper_item_fetcher_free(whc.fetcher);
}

void Meanings::edit_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();
 
    forms::EditMeaning formEdit;
    formEdit.load(context());

    if (!formEdit.validate()) {
        go_back_to_previous_page();
        return;
    }

    std::string defLang = formEdit.definitionLang.selected_id();
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
    TatoHyperItemFetcher *fetcher = wordsModel.get_word_with_id(origWordId);
    TatoHyperItem *word = fetcher->items[0];

    if (word != NULL) {
        response().set_redirect_header(
        "/" + get_interface_lang() +"/words/show-in" +
        "/" + std::string (word->str) +
        "/" + std::string (word->lang->code)
        );
    } else {
        go_back_to_previous_page();
    }
   
    tato_hyper_item_fetcher_free(fetcher);

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


