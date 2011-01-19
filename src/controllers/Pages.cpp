#include "Controller.h"
#include "controllers/Pages.h"
#include "shanghainesedict.h"
#include "contents/homepage.h"

namespace controllers {

Pages::Pages(apps::Shanghainesedict& shdictapp) : controllers::Controller(shdictapp) {
    shdictapp.dispatcher().assign("", &Pages::homepage, this);
}

void Pages::homepage() {
	contents::Homepage c;
    c.text = "plop";
    initContent(c);
	render("homepage", c);
}

} // End namespace
