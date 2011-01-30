#include "Controller.h"
#include "controllers/Pages.h"
#include "shanghainesedict.h"
#include "contents/homepage.h"

namespace controllers {

Pages::Pages(cppcms::service &serv) : controllers::Controller(serv) {
    dispatcher().assign("", &Pages::homepage, this);
}

void Pages::homepage() {
	contents::Homepage c;
    init_content(c);
	render("homepage", c);
}

} // End namespace
