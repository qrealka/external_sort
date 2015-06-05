#ifndef EXTERNAL_SORT_FILEMERGER_H
#define EXTERNAL_SORT_FILEMERGER_H

#include "FileWrapper.h"

namespace external_sort
{

class FileSplitter;

class FileMerger {

public:
    explicit FileMerger(const char* outFileName);

	void Merge(FileSplitter &splitter);

private:
    FileWrapper m_outFile;
};

}

#endif //EXTERNAL_SORT_FILEMERGER_H
