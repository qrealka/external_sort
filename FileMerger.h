#ifndef EXTERNAL_SORT_FILEMERGER_H
#define EXTERNAL_SORT_FILEMERGER_H

#include "FileWrapper.h"
#include "FileSplitter.h"

namespace external_sort
{

class FileMerger {

public:
    explicit FileMerger(const char* outFileName);

	void Merge(FileSplitter::SplitIterator begin, FileSplitter::SplitIterator end);

private:
    FileWrapper m_outFile;
};

}

#endif //EXTERNAL_SORT_FILEMERGER_H
