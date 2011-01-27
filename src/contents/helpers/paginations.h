#ifndef CONTENTS_HELPER_PAGINATIONS_H
#define CONTENTS_HELPER_PAGINATIONS_H

#include <cmath>
#include <cppcms/view.h>
extern "C" {
    #include "tato/hyperitem.h"
    #include "tato/fetcher.h"
}

#include "models/Translations.h"

#include "contents/Config.h"
namespace contents {

struct PaginationsHelper : public cppcms::base_content {
    std::string baseUrl;
    std::string lang;

    int offset;
    int size;
    int max; 
    int currentPage;
    int lastOffset; 
    int nextOffset;
    int prevOffset;
    int firstDisplayedPage;
    int lastDisplayedPage;
    bool atBeginning;
    bool atEnd;

    public:
        PaginationsHelper(
            int _offset, 
            int _size,
            int _max,
            std::string _baseUrl,
            std::string _lang
        ) {
            baseUrl = _baseUrl;
            max = _max;
            offset = _offset;
            size = _size;
            lang = _lang;
            prevOffset = 1 + offset - size;
            nextOffset = 1 + offset + size;
            lastOffset = 1 + max - (max%size);

            currentPage = offset/size + 1;
            int maxPage = ceil((double)max /(double)size);
            int paginationSize = std::min(6, maxPage-1);
            int maxPrevPages = std::min(paginationSize, currentPage-1);
            int maxNextPages = std::min(paginationSize, maxPage-currentPage);

            int minPrevPages = std::min((int)ceil((double)paginationSize/2.0), maxPrevPages);
            int minNextPages = std::min((int)floor((double)paginationSize/2.0), maxNextPages);

            int dispPrevPages;
            int dispNextPages;
            if (minPrevPages  <= minNextPages) {
                dispPrevPages = minPrevPages;
                dispNextPages = paginationSize - minPrevPages;
            } else {
                dispNextPages = minNextPages;
                dispPrevPages = paginationSize - minNextPages;
            }

            firstDisplayedPage = currentPage - dispPrevPages;
            lastDisplayedPage  = currentPage + dispNextPages;
            
            atEnd = (offset + size) >= max;
            atBeginning = currentPage <= 1 ; 

        }

        int offsetPage(int pageNumber) {
            return (pageNumber -1)*size + 1;
        }
};

}

#endif

