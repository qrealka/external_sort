#include "SortedFile.h"
#include "ThrowError.h"
#include "Chm.hpp"

namespace external_sort
{

SortedFile::SortedFile() : m_file(), m_offsets()
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

    m_file.Rewind();
    m_file.Write(  RangeConstChar(lines.front().begin(), lines.back().end()) );
    m_first.clear();
}

const CharBuffer& SortedFile::GetFirst() const {
    if (m_offsets.empty()) {
        m_first.clear();
        return m_first;
    }

    if (m_first.size())
        return m_first;

    const auto filePosition = m_offsets.crbegin()->second; // CHM builf graph in DESC order
    const size_t lineSize = filePosition.second;
    CHECK_CONTRACT(lineSize > 0, "Found empty line in chunk");

    m_first.resize(lineSize, 0);
    m_file.Read(filePosition.first, filePosition.second, m_first);

    return m_first;
}

bool SortedFile::Pop() {
    m_first.clear();

    if (m_offsets.empty())
        return false;

	m_offsets.erase(--m_offsets.cend());
    return true;
}

} // external_sort