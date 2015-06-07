#ifndef EXTERNAL_SORT_FILESPLITTER_H
#define EXTERNAL_SORT_FILESPLITTER_H

#include "FileWrapper.h"
#include "SortedFile.h"

#if defined(_MSC_VER)
#include <cstdint>
#endif
#include <list>
#include <functional>

namespace external_sort
{

class FileSplitter {

public:
	typedef std::list<SortedFile>  SplitContainer;
	typedef SplitContainer::iterator SplitIterator;
	typedef std::function<void(SplitIterator, SplitIterator)> AfterSplitCallback;

    explicit FileSplitter(const char* inputFileName);

	void Split(const size_t splitSize, AfterSplitCallback onAfterSplit);

private:
    FileWrapper m_file;
	SplitContainer m_parts;
};

} // external_sort


#endif //EXTERNAL_SORT_FILESPLITTER_H
