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

  	disp->assign("/edit/(\\d+)/(.*)$", &Metas::edit, this, 1, 2);
    disp->assign("/edit-treat$", &Metas::edit_treat, this);

  	disp->assign("/remove-from/(\\d+)/(.*)$", &Metas::remove_from, this, 1, 2);
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
        return ;
    }
    // if something is wrong we redirect to the previous page
    response().set_redirect_header(
        request().http_referer()
    );
}
/**
 *
 */

void Metas::edit(std::string wordId, std::string key) {
	int id = atoi(wordId.c_str());

	contents::MetasEdit c;
    initContent(c);
	contents::WordsHelper whc;
    whc.lang = c.lang;
    whc.fetcher = wordModel.get_word_with_id(id);


    if (whc.is_empty() || !metasModel.has_meta(id, key)) {
        response().set_redirect_header(
            request().http_referer()
        );
    } else {
        c.key = key;
        c.editMeta.wordId.value(wordId);
        c.editMeta.key.value(key);
        c.editMeta.value.value(
            metasModel.get_meta_from_key(id, key)
        );
        c.whc = whc;

        render("metas_edit",c);
    }
    tato_hyper_item_fetcher_free(whc.fetcher);

}

/**
 * 
 */
void Metas::edit_treat() {


	contents::MetasEdit c;
    initContent(c);
    c.editMeta.load(context());
    
    if (c.editMeta.validate()) {
       
        std::string wordId = c.editMeta.wordId.value();
        // TODO : handle if something wrong happen while saving
        metasModel.edit_meta(
            atoi(wordId.c_str()),
            c.editMeta.key.value(),
            c.editMeta.value.value()
        );

        response().set_redirect_header(
            "/" + c.lang +"/metas/add/" + wordId 
        );
        return;
    }
    // if something is wrong we redirect to the previous page
    response().set_redirect_header(
        request().http_referer()
    );



}
/**
 *
 */
void Metas::remove_from(std::string wordId, std::string key) {

	contents::MetasAdd c;
    initContent(c);
    
    //TODO test before if we really have an integer inside the string
    //TODO handle return value    
    metasModel.remove_meta(
        atoi(wordId.c_str()),
        key     
    );

    response().set_redirect_header(
        "/" + c.lang +"/metas/add/" + wordId
    );
}




} // End namespace

