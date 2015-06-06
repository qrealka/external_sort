#include "SortedFile.h"
#include "FileWrapper.h"
#include "ThrowError.h"
#include "Chm.hpp"

namespace external_sort
{

SortedFile::SortedFile(int64_t offset) : m_startChunkPosition(offset)
{

}

void SortedFile::SaveLines(const RangeLines& lines) {
    if (lines.empty())
        return;

    NMphf::Chm  hashes;
    CHECK_CONTRACT(hashes.generate(lines), "Cannot generate preserve order hash keys");
    const auto fileBegin = lines.front().begin();

    std::for_each(lines.begin(), lines.end(), [&hashes, this, fileBegin](const RangeConstChar& line) {
        unsigned hash;
        CHECK_CONTRACT(hashes.search(line, hash), "Cannot find hash for string");

        const auto fileOffset = line.begin() - fileBegin;
        m_offsets.emplace(hash, std::make_pair(fileOffset, line.size()));
    });

     m_first.clear();
}

const CharBuffer& SortedFile::GetFirst(const FileWrapper& file) const {
    if (m_offsets.empty()) {
        m_first.clear();
        return m_first;
    }

    if (m_first.size())
        return m_first;

    const auto filePosition = m_offsets.cbegin()->second; 
    const size_t lineSize = filePosition.second;
    CHECK_CONTRACT(lineSize > 0, "Found empty line in chunk");

    m_first.resize(lineSize, 0);
	const size_t bytes = file.Read(m_startChunkPosition + filePosition.first, m_first.data(), filePosition.second);
	assert(bytes);

	if (bytes < m_first.size()) {
		m_first.erase(m_first.begin() + bytes, m_first.end());
	}
    return m_first;
}

bool SortedFile::Pop() {
    m_first.clear();

    if (m_offsets.empty())
        return false;

	m_offsets.erase(m_offsets.cbegin());
    return true;
}

} // external_sort