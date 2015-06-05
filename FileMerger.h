#ifndef EXTERNAL_SORT_FILEMERGER_H
#define EXTERNAL_SORT_FILEMERGER_H

#include "FileWrapper.h"
#include <list>

namespace external_sort
{

class FileMerger {

public:
    explicit FileMerger(const char* outFileName);

    void Merge(const std::list<external_sort::FileWrapper> &list);
    void Final();

private:
    FileWrapper m_outFile;
};

}

#endif //EXTERNAL_SORT_FILEMERGER_H
