#include "FileSplitter.h"
#include "ThrowError.h"
#include <limits>

namespace
{

const size_t MaxStringLength = MAX_STRING_SIZE * 1024;
const int64_t MaxMemoryAlloc = std::numeric_limits<int>::max();

const char* GetLines(const char* begin, const char* end, external_sort::RangeLines& lines){
    const char* result = begin;
    lines.clear();

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
{
    static_assert(MAX_STRING_SIZE > 0, "Limit of string length cannot be zero");
}

void FileSplitter::Split(int64_t splitSize) {

    CHECK_CONTRACT(splitSize > MaxStringLength, "too low split size specified");
    CHECK_CONTRACT(splitSize < (MaxMemoryAlloc - MaxStringLength), "too high split size specified");

    m_parts.clear();
    if (!m_file.GetFileSize())
        return;

    const auto chunkSize = static_cast<size_t>(splitSize);
    std::vector<char>  chunk(chunkSize + MaxStringLength); // maximum size (in GB) of part of input file fol sorting
	char* buffer = chunk.data();

    m_parts.emplace_back(""); // create TEMP file
    RangeLines lines;

    for (int64_t position = 0;;) {
        const auto bufferLength = m_file.ReadChunk(position, buffer, chunkSize);
        const char* chunkEnd = buffer + bufferLength;
        const char* bufferEnd = GetLines(chunk.data(), chunkEnd, lines);

        assert(bufferEnd > chunk.data());
        position += splitSize;

        // EOF ?
        if (bufferLength < splitSize || position >= m_file.GetFileSize()) {
            // put last line to result
            lines.emplace_back(bufferEnd, chunkEnd);
			SortChunk(m_parts.back(), lines);
            break; // split done
        }

		SortChunk(m_parts.back(), lines);
        m_parts.emplace_back(""); // new TEMP file (split part)

        // part of string ('newline' not found) copy to head of buffer
        buffer = std::copy_n(bufferEnd, chunkEnd - bufferEnd, chunk.data());
    }
}

void FileSplitter::SortChunk(FileWrapper& file, RangeLines& lines) {
	std::sort(lines.begin(), lines.end());
	std::for_each(lines.begin(), lines.end(), [&file](RangeConstChar& range){
		file.Write(range);
	});
}

const RangeConstChar& FileSplitter::FindNextMinimum() const {
	
}

} // external_sort
