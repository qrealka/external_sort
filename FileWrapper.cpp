#include "FileWrapper.h"
#include "ThrowError.h"
#include <sys/types.h>
#include <sys/stat.h>

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64)
#define stat64 _stat64
#define status_t struct _stat64
#define fseek64 _fseeki64
#else
#define status_t struct stat64
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

    m_file = std::fopen(fileName, input ? "rb" : "wb");
    CHECK_CONTRACT(m_file, std::string("Cannot open file ") + fileName);

    if (input) {
        status_t fileStat;
        CHECK_CONTRACT(!stat64(fileName, &fileStat), std::string("Cannot get file size ") + fileName);

        m_size = fileStat.st_size;
        CHECK_CONTRACT(m_size > 0 && MaxFileSize >= m_size, std::string("Too large file ") + fileName);
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
        size_t bytes = std::fread(chunk, sizeof(char), chunkSize, m_file);
        if (bytes)
            return bytes;
    }
    CHECK_CONTRACT(std::feof(m_file), "Cannot read from file");
    return 0;
}

void FileWrapper::WriteChunk(size_t offset, const std::vector<char> &chunk) {

}

void FileWrapper::Write(const RangeConstChar &range) {
    assert(m_file);
    CHECK_CONTRACT(range.size() == std::fwrite(range.begin(), sizeof(char), range.size(), m_file), "Cannot write to file");
    std::fputc('\n', m_file);
    m_size += range.size() + 1; // CRLF windows ignore
}

int64_t FileWrapper::GetFileSize() const {
    return m_size;
}

void FileWrapper::Close() {
    if (m_file)
        fclose(m_file);
    m_file = nullptr;
}

void FileWrapper::CopyFileTo(FileWrapper& destFile) const {
    CHECK_CONTRACT(m_file && destFile.m_file, "Cannot copy closed files");
    rewind(m_file);
    rewind(destFile.m_file);
    std::vector<char> buffer(std::min<int64_t>(m_size, FILE_CHUNK_SIZE * 1024 * 1024)); // can be int overflow on x64

    while(!feof(m_file)) {
        size_t bytes = std::fread(buffer.data(), sizeof(char), buffer.size(), m_file);
        if (!bytes)
            break;
        std::fwrite(buffer.data(), sizeof(char), bytes, destFile.m_file);
        CHECK_CONTRACT(!std::ferror(destFile.m_file), "Error write file");
    }
}

FileWrapper::FileWrapper(const external_sort::FileWrapper &wrapper) {
    assert(false && "FileWrapper disable copy constructor");
}

void FileWrapper::operator=(const external_sort::FileWrapper &wrapper) {
    assert(false && "FileWrapper disable assign operator");
}

} // namespace
