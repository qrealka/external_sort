#ifndef EXTERNAL_SORT_FILEWRAPPER_H
#define EXTERNAL_SORT_FILEWRAPPER_H

#include <string>
#include <vector>
#include <cstdio> // FILE
#include "Range.h"

namespace external_sort
{

class FileWrapper {
public:
    FileWrapper(); // temporary file
    FileWrapper(const std::string& fileName, bool input);
    ~FileWrapper();

    size_t ReadChunk(size_t offset, std::vector<char>& chunk);
    void WriteChunk(size_t offset, const std::vector<char>& chunk);
    void Write(const RangeConstChar& range);
private:
    FileWrapper(const FileWrapper&);
    void operator=(const FileWrapper&);

private:
    std::FILE* m_file;
};

}

#endif //EXTERNAL_SORT_FILEWRAPPER_H
