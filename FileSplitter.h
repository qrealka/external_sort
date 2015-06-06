#ifndef EXTERNAL_SORT_FILESPLITTER_H
#define EXTERNAL_SORT_FILESPLITTER_H

#include "FileWrapper.h"
#include "SortedFile.h"

#if defined(_MSC_VER)
#include <cstdint>
#endif
#include <list>

namespace external_sort
{

class FileMerger;

class FileSplitter {

public:
	typedef std::list<SortedFile>  SplitContainer;
	typedef SplitContainer::iterator SplitIterator;

    explicit FileSplitter(const char* inputFileName);

    void Split(size_t splitSize);
    void Merge(FileMerger& merger);

private:
    FileWrapper m_file;
	SplitContainer m_parts;
};

} // external_sort


#endif //EXTERNAL_SORT_FILESPLITTER_H
