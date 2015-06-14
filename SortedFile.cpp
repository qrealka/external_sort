#include "SortedFile.h"
#include "FileWrapper.h"
#include "ThrowError.h"

#include <algorithm>

namespace external_sort
{

SortedFile::SortedFile(const FileWrapper& file, int64_t offset) : m_file(file), m_startChunkPosition(offset)
{

}

void SortedFile::SaveLines(RangeLines& lines) {
    if (lines.empty())
        return;

	const auto fileBegin = lines.front().begin();

	std::sort(lines.begin(), lines.end());
    std::for_each(lines.begin(), lines.end(), [this, fileBegin](const RangeConstChar& line) {
 
        const auto fileOffset = line.begin() - fileBegin;
#ifdef DEBUG_MERGE
		assert(line.size() == DEBUG_MERGE);
#endif
        m_offsets.emplace_back(fileOffset, line.size());
    });

	//std::reverse(m_offsets.begin(), m_offsets.end());
    m_first.clear();
}

const CharBuffer& SortedFile::GetFirst() const {
    if (m_offsets.empty()) {
        m_first.clear();
        return m_first;
    }

	if (m_first.size())
	{
#ifdef DEBUG_MERGE
		assert(m_first.size() == DEBUG_MERGE);
#endif
		return m_first;
	}

    const auto& filePosition = m_offsets.front(); 
    const uint16_t lineSize = filePosition.second;
    CHECK_CONTRACT(lineSize > 0, "Found empty line in chunk");

#ifdef DEBUG_MERGE
	assert(lineSize == DEBUG_MERGE);
#endif

    m_first.resize(lineSize, 0);
	const size_t bytes = m_file.Read(m_startChunkPosition + filePosition.first, m_first.data(), filePosition.second);
#ifdef DEBUG_MERGE
	assert(bytes == DEBUG_MERGE);
#else
	assert(bytes);
#endif

	if (bytes < m_first.size()) {
		m_first.erase(m_first.begin() + bytes, m_first.end());
	}
    return m_first;
}

bool SortedFile::Pop() {
    m_first.clear();
	m_offsets.pop_front();
	return !m_offsets.empty();
}

} // external_sort