#include "SortedFile.h"
#include "ThrowError.h"
#include "Chm.hpp"

namespace external_sort
{

void SortedFile::SaveLines(const RangeLines& lines) {
    if (lines.empty())
        return;

    NMphf::Chm  hashes;
    CHECK_CONTRACT(hashes.generate(lines), "Cannot generate preserve order hash keys");
    const auto fileBegin = lines.front().begin();

    std::for_each(lines.begin(), lines.end(), [&hashes, &m_offsets, fileBegin](RangeConstChar& line) {
        unsigned hash;
        CHECK_CONTRACT(hashes.search(line, hash), "Cannot find hash for string");

        const auto fileOffset = line.begin() - fileBegin;
        m_offsets.emplace(hash, std::make_pair(fileOffset, line.size()));
    });

    m_file.Rewind();
    m_file.Write(  RangeConstChar(lines.front().begin(), lines.back().end()) );
}

std::string SortedFile::GetFirst() const {
    if (m_offsets.empty())
        return std::string();

    const auto filePosition = m_offsets.begin()->second;
    const size_t lineSize = filePosition.second;
    CHECK_CONTRACT(lineSize > 0, "Found empty line in chunk");

    std::string line(lineSize, 0);
    m_file.Read(filePosition.first, filePosition.second, line);

    return line;
}

bool SortedFile::Pop() {
    if (m_offsets.empty())
        return false;

	m_offsets.erase(m_offsets.cbegin());
    return true;
}

} // external_sort