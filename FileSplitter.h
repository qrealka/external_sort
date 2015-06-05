#ifndef EXTERNAL_SORT_FILESPLITTER_H
#define EXTERNAL_SORT_FILESPLITTER_H

#include "FileWrapper.h"
#include "Range.h"
#include <list>

#if defined(_MSC_VER)
#include <cstdint>
#endif

namespace external_sort
{

class FileSplitter {

public:
    typedef void (*OnSplitCallback)(FileWrapper&, RangeLines&);
    explicit FileSplitter(const char* inputFileName);

    void Split(int64_t splitSize, OnSplitCallback onSplit);
    const std::list<FileWrapper>& GetSplitResults() const;

private:
    FileWrapper m_file;
    std::list<FileWrapper> m_parts;
};

} // external_sort


#endif //EXTERNAL_SORT_FILESPLITTER_H
