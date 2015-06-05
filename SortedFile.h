#ifndef EXTERNAL_SORT_FILEINDEX_H
#define EXTERNAL_SORT_FILEINDEX_H

#include "FileWrapper.h"
#include <map>

namespace external_sort
{

class SortedFile {
public:
	SortedFile();
	void SaveLines(const RangeLines& lines);
	const CharBuffer& GetFirst() const;
	bool Pop();

private:
	SortedFile(const SortedFile&);
	void operator=(const SortedFile&);

private:
	FileWrapper m_file;
	std::map<unsigned, std::pair<size_t, size_t> > m_offsets;
	mutable CharBuffer m_first; // cache read file operation
};

} // external_sort

#endif //EXTERNAL_SORT_FILEINDEX_H
