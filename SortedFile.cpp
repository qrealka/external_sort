#include "SortedFile.h"
#include "ThrowError.h"

namespace external_sort
{

SortedFile::SortedFile(const FileWrapper& file, int64_t offset)
        : m_tempFile()
        , m_linesCount(0)
        , m_inputFile(file)
        , m_startChunkPosition(offset)
{

}

void SortedFile::SaveLines(RangeLines& lines) {
    if (lines.empty())
        return;

	const auto fileBegin = lines.front().begin();

	std::sort(lines.begin(), lines.end());
    std::for_each(lines.begin(), lines.end(), [this, fileBegin](const RangeConstChar& line) {
 
        const size_t fileOffset = line.begin() - fileBegin;
#ifdef DEBUG_MERGE
		assert(line.size() == DEBUG_MERGE);
#endif
        size_t pair[] = {fileOffset, line.size()};
        m_tempFile.WriteNumbers(pair);

    });

    m_linesCount = lines.size();
    m_first.clear();
    m_tempFile.Rewind();
}

const CharBuffer& SortedFile::GetFirst() const {
    if (!m_linesCount) {
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

    size_t filePosition[] = {0,0};
    m_tempFile.ReadNumbers(filePosition);
    const uint16_t lineSize = filePosition[1];
    CHECK_CONTRACT(lineSize > 0, "Found empty line in chunk");

#ifdef DEBUG_MERGE
	assert(lineSize == DEBUG_MERGE);
#endif

    m_first.resize(lineSize, 0);
	const size_t bytes = m_inputFile.Read(m_startChunkPosition + filePosition[0], m_first.data(), filePosition[1]);
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
    if (m_tempFile.IeEOF()) {
        m_tempFile.Close();
        return true;
    }
	return false;
}

} // external_sort