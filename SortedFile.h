#ifndef EXTERNAL_SORT_FILEINDEX_H
#define EXTERNAL_SORT_FILEINDEX_H

#include "Range.h"
#include "FileWrapper.h"

#if defined(_MSC_VER)
#include <cstdint>
#endif
#include <list>

namespace external_sort
{

class FileWrapper;

class SortedFile {
public:
	explicit SortedFile(const FileWrapper& file, int64_t offset);

	void SaveLines(RangeLines& lines);
	const CharBuffer& GetFirst() const;
	bool Pop();

private:
	SortedFile(const SortedFile&);
	void operator=(const SortedFile&);

private:
	typedef std::pair<size_t, uint16_t> OffsetType;
	typedef std::list<OffsetType> OffsetsList;

	const FileWrapper&  m_file;
	int64_t m_startChunkPosition;
	OffsetsList m_offsets;
	mutable CharBuffer m_first; // cache read file operation
};

} // external_sort

#endif //EXTERNAL_SORT_FILEINDEX_H
