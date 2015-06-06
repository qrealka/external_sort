#ifndef EXTERNAL_SORT_FILEINDEX_H
#define EXTERNAL_SORT_FILEINDEX_H

#include "Range.h"

#if defined(_MSC_VER)
#include <cstdint>
#endif
#include <map>
#include <functional>

namespace external_sort
{

class FileWrapper;

class SortedFile {
public:
	explicit SortedFile(int64_t offset);

	void SaveLines(const RangeLines& lines);
	const CharBuffer& GetFirst(const FileWrapper& file) const;
	bool Pop();

private:
	int64_t m_startChunkPosition;
	std::map<unsigned, std::pair<size_t, size_t>, std::greater<unsigned> > m_offsets; // CHM builf graph in DESC order
	mutable CharBuffer m_first; // cache read file operation
};

} // external_sort

#endif //EXTERNAL_SORT_FILEINDEX_H
