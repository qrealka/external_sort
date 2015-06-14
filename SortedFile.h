#ifndef EXTERNAL_SORT_FILEINDEX_H
#define EXTERNAL_SORT_FILEINDEX_H

#include "Range.h"
#include "FileWrapper.h"

#if defined(_MSC_VER)
#include <cstdint>
#endif

namespace external_sort
{

class FileWrapper;

class SortedFile {
public:
    SortedFile(const FileWrapper& file, int64_t offset);

	void SaveLines(RangeLines& lines);
	const CharBuffer& GetFirst() const;
	bool Pop();

private:
	SortedFile(const SortedFile&);
	void operator=(const SortedFile&);

private:
	FileWrapper m_tempFile;
	size_t m_linesCount;
	const FileWrapper& m_inputFile;
	int64_t m_startChunkPosition;
	mutable CharBuffer m_first; // cache read file operation
};

} // external_sort

#endif //EXTERNAL_SORT_FILEINDEX_H
