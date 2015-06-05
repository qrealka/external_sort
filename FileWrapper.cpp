#include "FileWrapper.h"
#include "ThrowError.h"

#include <sys/types.h>
#include <sys/stat.h>


#if defined(_MSC_VER)

#define stat64 _stat64
#define status_t _stat64
#define fseek64 _fseeki64

#elif defined(__MINGW32__)

#define stat64 _stat64
#define status_t __stat64
#define fseek64 _fseeki64
#else

#define status_t stat64

#endif

namespace
{

const int64_t MaxFileSize = MAX_INPUT_FILE_SIZE * 1024LL * 1024LL * 1024LL;

} //namespace


namespace external_sort
{

FileWrapper::FileWrapper(const char*)
        : m_file(nullptr)
        , m_size(0)
{
    m_file = std::tmpfile();
    CHECK_CONTRACT(m_file, "Cannot create temporary file");
}

FileWrapper::FileWrapper(const char *fileName, bool input)
        : m_file(nullptr)
        , m_size(0)
{
    static_assert(MAX_INPUT_FILE_SIZE > 0, "File size limit cannot be zero");
    CHECK_CONTRACT(fileName && fileName[0], "File name is empty");

    m_file = std::fopen(fileName, input ? "rb" : "ab");
    CHECK_CONTRACT(m_file, std::string("Cannot open file ") + fileName);

    if (input) {
        struct status_t fileStat;
        CHECK_CONTRACT(!stat64(fileName, &fileStat), std::string("Cannot get file size ") + fileName);

        m_size = fileStat.st_size;
		CHECK_CONTRACT(m_size > 0, std::string("Empty input file ") + fileName);
		CHECK_CONTRACT(MaxFileSize >= m_size, std::string("Too large file ") + fileName);
    }
}

FileWrapper::~FileWrapper() {
    Close();
}

size_t FileWrapper::ReadChunk(int64_t offset, char* const chunk, size_t chunkSize) {
    assert(chunk && chunkSize);
    assert(m_file);
    assert(offset >= 0);

    if (!fseek64(m_file, offset, SEEK_SET)) {
        const size_t bytes = std::fread(chunk, sizeof(char), chunkSize, m_file);
        if (bytes)
            return bytes;
    }
    CHECK_CONTRACT(std::feof(m_file), "Cannot read from file");
    return 0;
}


void FileWrapper::Write(const RangeConstChar &range) {
    assert(m_file);
    CHECK_CONTRACT(range.size() == std::fwrite(range.begin(), sizeof(char), range.size(), m_file), "Cannot write to file");
    std::fputc('\n', m_file);
}

void FileWrapper::Close() {
    if (m_file)
        fclose(m_file);
    m_file = nullptr;
}

int64_t FileWrapper::GetFileSize() const {
	return m_size;
}

void FileWrapper::Rewind() const {
	rewind(m_file);
}

FileWrapper::FileWrapper(const external_sort::FileWrapper &wrapper) {
    assert(false && "FileWrapper disable copy constructor");
}

void FileWrapper::operator=(const external_sort::FileWrapper &wrapper) {
    assert(false && "FileWrapper disable assign operator");
}

} // namespace
