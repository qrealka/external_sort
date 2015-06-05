#ifndef EXTERNAL_SORT_FILEWRAPPER_H
#define EXTERNAL_SORT_FILEWRAPPER_H

#if defined(__MINGW32__)
#define __MSVCRT_VERSION__ 0x900
#elif defined(_MSC_VER)
#include <cstdint>
#endif

#include <stdio.h>
#include "Range.h"

namespace external_sort
{

	;

class FileWrapper {
public:
    FileWrapper(); // temporary file
    FileWrapper(const char* fileName, bool input);
    ~FileWrapper();

    size_t ReadChunk(int64_t offset, char* const chunk, size_t chunkSize);
    void Write(const RangeConstChar& range);
    void Close();

    int64_t GetFileSize() const;
	void Rewind() const;

    void Read(size_t offset, size_t size, std::string &out) const;

private:
    FileWrapper(const FileWrapper&);
    void operator=(const FileWrapper&);

private:
    std::FILE* m_file;
    int64_t m_size;
};

}

#endif //EXTERNAL_SORT_FILEWRAPPER_H
