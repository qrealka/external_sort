#ifndef EXTERNAL_SORT_FILESPLITTER_H
#define EXTERNAL_SORT_FILESPLITTER_H

#include "FileWrapper.h"
#include "SortedFile.h"

#if defined(_MSC_VER)
#include <cstdint>
#endif

namespace external_sort
{

class FileSplitter {

public:
    explicit FileSplitter(const char* inputFileName);

    void Split(size_t splitSize);
    CharBuffer FindNextMinimum();

private:
    FileWrapper m_file;
    std::vector<SortedFile> m_parts;
};

} // external_sort


#endif //EXTERNAL_SORT_FILESPLITTER_H
