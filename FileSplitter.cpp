#include "FileSplitter.h"
#include "ThrowError.h"
#include "Range.h"
#include <limits>

namespace
{

const size_t MaxStringLength = MAX_STRING_SIZE * 1024;
const int64_t MaxMemoryAlloc = std::numeric_limits<int>::max();

const char* GetLines(const char* begin, const char* end, external_sort::RangeLines& lines){
	assert(begin && end);
	assert(begin <= end);

	auto result = begin;
    lines.clear();

    for (auto it = begin; it < end ;) {
        CHECK_CONTRACT(it - begin <= MaxStringLength, "Too big string!");

        // CRLF windows hell
		if (*it == 13 || *it == 10) {
			// ignore empty lines ??
			if (begin != it)
				lines.emplace_back(begin, it);

			// skip many CRLFs
			for (; (*it == 13 || *it == 10) && it < end; ++it);
			begin = result = it;
		} else {
			++it;
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

    m_parts.emplace_back(); // create TEMP file
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
			m_parts.back().SaveLines(lines);
            break; // split done
        }

		m_parts.back().SaveLines(lines);
        m_parts.emplace_back(); // new TEMP file (split part)

        // part of string ('newline' not found) copy to head of buffer
        buffer = std::copy_n(bufferEnd, chunkEnd - bufferEnd, chunk.data());
    }
}

CharBuffer FileSplitter::FindNextMinimum() {
    if (m_parts.empty())
        return CharBuffer();

    const auto min = std::min_element(m_parts.begin(), m_parts.end(), [](const SortedFile& current, const SortedFile& smallest){
        const auto curValue = current.GetFirst();
        const auto minValue = smallest.GetFirst();
        return std::lexicographical_compare(curValue.begin(), curValue.end(),
                                            minValue.begin(), minValue.end());
    });

    CharBuffer result = min->GetFirst();
    min->Pop();
    return result;
}

} // external_sort
