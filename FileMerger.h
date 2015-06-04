#ifndef EXTERNAL_SORT_FILEMERGER_H
#define EXTERNAL_SORT_FILEMERGER_H

#include "FileWrapper.h"
#include <string>
#include <list>

namespace external_sort
{

class FileMerger {

public:
    explicit FileMerger(const std::string &outFileName);

    void Merge(const std::list<external_sort::FileWrapper> &list);
    void Final();
};

}

#endif //EXTERNAL_SORT_FILEMERGER_H
