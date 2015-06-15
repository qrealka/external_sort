#include "FileSplitter.h"
#include "ThrowError.h"
#include "Range.h"
#include <limits>

namespace
{

#ifdef DEBUG_MERGE
const size_t MaxStringLength = 256;
#else
const size_t MaxStringLength = MAX_STRING_SIZE * 1024;
#endif

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
			{
#ifdef DEBUG_MERGE
				assert((it - begin) == DEBUG_MERGE);
#endif
				lines.emplace_back(begin, it);
			}

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

void FileSplitter::SplitImpl(const size_t splitSize)
{
	std::vector<char>  chunk(splitSize + MaxStringLength); // maximum size (in GB) of part of input file for sorting

	int64_t position = 0LL;
	m_parts.emplace_back(m_file, position); // create split

	RangeLines lines;
	lines.reserve(chunk.size() / sizeof(RangeConstChar));

	for (const auto buffer = chunk.data();;) {
		const auto bufferLength = m_file.Read(position, buffer, splitSize);
		const char* chunkEnd = buffer + bufferLength;
		const char* bufferEnd = GetLines(buffer, chunkEnd, lines);

		assert(bufferEnd > chunk.data());
		position += bufferLength;

		// EOF ?
		if (bufferLength < splitSize || position >= m_file.GetFileSize()) {
			// put last line to result
			if (bufferEnd != chunkEnd)
				lines.emplace_back(bufferEnd, chunkEnd);
			m_parts.back().SaveLines(lines);
			break; // split done
		}

		m_parts.back().SaveLines(lines);
		position -= chunkEnd - bufferEnd;

		m_parts.emplace_back(m_file, position); // new split 
	}
}

void FileSplitter::Split(const size_t splitSize, AfterSplitCallback onAfterSplit) {

    CHECK_CONTRACT(splitSize > MaxStringLength, "too low split size specified");
    CHECK_CONTRACT(splitSize < (MaxMemoryAlloc - MaxStringLength), "too high split size specified");

    m_parts.clear();
	if (m_file.GetFileSize())
	{
		SplitImpl(splitSize);

		if (!m_parts.empty())
			onAfterSplit(m_parts.begin(), m_parts.end());
	}
}


} // external_sort
