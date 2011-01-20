#ifndef CONTENTS_WORDS_H
#define CONTENTS_WORDS_H

#include "contents/content.h"

extern "C" {
#include <tato/fetcher.h>
#include <tato/hyperitem.h>
}

namespace contents {

struct Words : public BaseContent {
    TatoHyperItemFetcher *fetcher;
};

}

#endif
