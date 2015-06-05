#ifndef EXTERNAL_SORT_FILEWRAPPER_H
#define EXTERNAL_SORT_FILEWRAPPER_H

#include <vector>
#include <cstdio> // FILE
#include "Range.h"

namespace external_sort
{

class FileWrapper {
public:
    explicit FileWrapper(const char*); // temporary file. input parameter not used
    FileWrapper(const char* fileName, bool input);
    ~FileWrapper();

    size_t ReadChunk(int64_t offset, char* const chunk, size_t chunkSize);
    void CopyFileTo(FileWrapper& destFile) const;
    void WriteChunk(size_t offset, const std::vector<char>& chunk);
    void Write(const RangeConstChar& range);
    void Close();
    int64_t GetFileSize() const;
private:
    FileWrapper(const FileWrapper&);
    void operator=(const FileWrapper&);

private:
    std::FILE* m_file;
    int64_t m_size;
};

}

#endif //EXTERNAL_SORT_FILEWRAPPER_H
