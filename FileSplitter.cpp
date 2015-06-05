#include "FileSplitter.h"
#include "ThrowError.h"

namespace
{

const size_t MaxFileChunkSize = FILE_CHUNK_SIZE  * 1024 * 1024;
const size_t MaxStringLength = MAX_STRING_SIZE * 1024;

const char* GetLines(const char* begin, const char* end, external_sort::RangeLines& lines){
    const char* result = begin;

    for (auto it = begin; it < end ; ++it) {
        CHECK_CONTRACT(it - begin <= MaxStringLength, "Too big string!");

        // CRLF windows hell
        if ((it[0] == 13 && it[1] == 10) || (it[0] == 10 && it[1] == 13)) {
            lines.emplace_back(begin, it);
            result = begin = it++ + 2;
        } else if (*it == 13) {
            lines.emplace_back(begin, it);
            result = begin = it + 1;
        }
    }

    return result > end ? end : result;
}

} // namespace

namespace external_sort
{

FileSplitter::FileSplitter(const char* inputFileName)
        : m_file(inputFileName, true)
        , m_chunk(MaxFileChunkSize + MaxStringLength) // bound of chunk can break string
{
    static_assert(FILE_CHUNK_SIZE > 0, "Buffer for file i/o cannot be zero length");
    static_assert(MAX_STRING_SIZE > 0, "Limit of string length cannot be zero");
    assert(MaxFileChunkSize > MaxStringLength);
}

void FileSplitter::Split(int64_t splitSize, OnSplitCallback onSplit) {

    RangeLines lines;
    char* buffer = m_chunk.data();

    m_parts.clear();
    if (!m_file.GetFileSize())
        return;

    m_parts.emplace_back(""); // create TEMP file

    for (int64_t position = 0;;) {
        const size_t bufferLength = m_file.ReadChunk(position, buffer, MaxFileChunkSize);
        const char* chunkEnd = buffer + bufferLength;
        const char* bufferEnd = GetLines(m_chunk.data(), chunkEnd, lines);

        assert(bufferEnd > m_chunk.data());
        position += MaxFileChunkSize;

        // EOF ?
        if (bufferLength < MaxFileChunkSize || position >= m_file.GetFileSize()) {
            // put last line to result
            lines.emplace_back(bufferEnd, chunkEnd);
            onSplit(m_parts.back(), lines);
            break; // split done
        }

        // part of string ('newline' not found) copy to head of buffer
        buffer = std::copy_n(bufferEnd, chunkEnd - bufferEnd, m_chunk.data());

        if (position >= m_parts.size() * splitSize) {
            onSplit(m_parts.back(), lines);
            m_parts.emplace_back(""); // new TEMP file (split part)
        }
    }
}

const std::list<FileWrapper>& FileSplitter::GetSplitResults() const {
    return m_parts;
}

} // external_sort
