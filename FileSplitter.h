#ifndef EXTERNAL_SORT_FILESPLITTER_H
#define EXTERNAL_SORT_FILESPLITTER_H

#include "FileWrapper.h"
#include "Range.h"
#include <string>
#include <list>

namespace external_sort
{

class FileSplitter {

public:
    typedef void (*OnSplitCallback)(FileWrapper&, RangeLines&);
    explicit FileSplitter(const std::string &inputFileName);

    void Split(int splitSize, OnSplitCallback onSplit);

    const std::list<FileWrapper>& GetSplitResults() const;

private:
    FileWrapper m_file;
    std::vector<char> m_chunk;
    std::list<FileWrapper> m_parts;
};

} // external_sort


#endif //EXTERNAL_SORT_FILESPLITTER_H
