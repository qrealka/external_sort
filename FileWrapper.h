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

class FileWrapper {
public:
	FileWrapper(); // temporary file
	FileWrapper(const char* fileName, bool input);
	FileWrapper(FileWrapper&& tmp);
    ~FileWrapper();

    size_t Read(int64_t offset, char* const chunk, size_t chunkSize) const;
    void Write(const RangeConstChar& range) const;
    void Close();

    int64_t GetFileSize() const;
	void Rewind() const;
    bool IeEOF() const;

    void WriteNumbers(size_t numbers[]) const;
    void ReadNumbers(size_t numbers[]) const;

private:
    FileWrapper(const FileWrapper&);
    void operator=(const FileWrapper&);

private:
    std::FILE* m_file;
    int64_t m_size;
};

}

#endif //EXTERNAL_SORT_FILEWRAPPER_H
