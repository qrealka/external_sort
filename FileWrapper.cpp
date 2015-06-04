#include "FileWrapper.h"
#include "ThrowError.h"

namespace external_sort
{

FileWrapper::FileWrapper() : m_file(nullptr) {
    m_file = std::tmpfile();
    if (!m_file)
        THROW_EXCEPTION(std::runtime_error, "Cannot create temporary file");
}

FileWrapper::FileWrapper(const std::string &fileName, bool input) : m_file(nullptr) {
    m_file = std::fopen(fileName.c_str(), input ? "rb" : "w+b");
    if (!m_file)
        THROW_EXCEPTION(std::runtime_error, std::string("Cannot open file ") + fileName );
}

FileWrapper::~FileWrapper() {
    if (m_file)
        fclose(m_file);
}

size_t FileWrapper::ReadChunk(size_t offset, std::vector<char> &chunk) {
    return 0;
}

void FileWrapper::WriteChunk(size_t offset, const std::vector<char> &chunk) {

}

void FileWrapper::Write(const RangeConstChar &range) {
    assert(m_file);
    if (range.size() != fwrite(range.begin(), sizeof(char), range.size(), m_file))
        THROW_EXCEPTION(std::runtime_error, "Cannot write to file");
}

FileWrapper::FileWrapper(const external_sort::FileWrapper &wrapper) {
    assert(false && "FileWrapper disable copy constructor");
}

void FileWrapper::operator=(const external_sort::FileWrapper &wrapper) {
    assert(false && "FileWrapper disable assign operator");
}


}
