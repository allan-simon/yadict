#include "Controller.h"
#include "controllers/Metas.h"
#include "shanghainesedict.h"
#include "contents/words.h"
#include "contents/metas.h"

#include "models/TatoHyperDB.h"

namespace controllers {

Metas::Metas(cppcms::service &serv) : Controller(serv) {
    // TODO have /  and /show directing to some "index" page 
    // as cburgmer pointed out, some people do navigate by crafting url
    cppcms::url_dispatcher* disp = &dispatcher();


  	disp->assign("/add/(\\d+)$", &Metas::add, this, 1);
  	disp->assign("/add-treat$", &Metas::add_treat, this);

  	//disp->assign("/edit/(\\d+)$", &Metas::edit, this, 1);
    //disp->assign("/edit-treat$", &Metas::edit_treat, this);

  	//disp->assign("/delete-by-id-name/(\\d+)/(.*)$", &Metas::delete_by_id_name, this, 1, 2);
}

/**
 * Display the page with a form to add new meta on a word
 */
void Metas::add(std::string wordId) {
	int id = atoi(wordId.c_str());

	contents::MetasAdd c;
    initContent(c);
	contents::WordsHelper whc;
    whc.lang = c.lang;
    whc.fetcher = wordModel.get_word_with_id(id);
    c.addMeta.wordId.value(wordId);
    c.whc = whc;


    if (whc.is_empty()) {
        response().set_redirect_header(
            request().http_referer()
        );
    } else {

        render("metas_add",c);
    }
    tato_hyper_item_fetcher_free(whc.fetcher);
}


/**
 * virtual page, used to received the data from the words/add form
 * and treat them
 */

void Metas::add_treat() {

	contents::MetasAdd c;
    initContent(c);
    c.addMeta.load(context());
    
    if (c.addMeta.validate()) {
       
        std::string wordId = c.addMeta.wordId.value();
        // TODO : handle if something wrong happen while saving
        metasModel.add_meta(
            atoi(wordId.c_str()),
            c.addMeta.key.value(),
            c.addMeta.value.value()
        );

        response().set_redirect_header(
            "/" + c.lang +"/metas/add/" + wordId 
        );

    }
    // if something is wrong we redirect to the previous page
    response().set_redirect_header(
        request().http_referer()
    );
}
/**
 * Display the page with a form to edit the information about a word
 */
/*
void Metas::edit(std::string wordId) {
	int id = atoi(wordId.c_str());

	contents::MetasEdit c;
	contents::MetasHelper whc;
    initContent(c);
    whc.lang = c.lang;

    whc.fetcher = wordModel.get_word_with_id(id);
    // if no item with this id
    if (whc.fetcher->items[0] == NULL) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        tato_hyper_item_fetcher_free(whc.fetcher);
        return;
    }

    // Set the value of the form to the current value
    // So one will know what he is editing
    c.editMeta.wordId.value(wordId);
    c.editMeta.wordLang.selected_id(
        std::string(whc.fetcher->items[0]->lang->code)
    );
    c.editMeta.wordString.value(
        std::string(whc.fetcher->items[0]->str)
    );


    c.whc = whc;
    render("words_edit",c);
    tato_hyper_item_fetcher_free(whc.fetcher);
}

*/
/**
 * virtual page, used to received the data from the words/edit form
 * and treat them
 */
/*
void Metas::edit_treat() {

	contents::MetasEdit c;
    initContent(c);
    c.editMeta.load(context());
    
    if (c.editMeta.validate()) {
        // TODO : handle if something wrong happen while saving
        wordModel.edit_word(
            atoi(c.editMeta.wordId.value().c_str()), 
            c.editMeta.wordLang.selected_id(),
            c.editMeta.wordString.value()
        );
    }

    response().set_redirect_header(
        "/" + c.lang +"/words/show-all"
    );
}
*/
/**
 * virtual page, delete the item with the given id
 */
/*
void Metas::delete_by_id(std::string wordId) {

	contents::MetasAdd c;
    initContent(c);
    
    //TODO test before if we really have an integer inside the string
    //TODO handle return value    
    wordModel.delete_word(atoi(wordId.c_str()));

    response().set_redirect_header(
        "/" + c.lang +"/words/show-all"
    );
}
*/




} // End namespace

