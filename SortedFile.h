#ifndef EXTERNAL_SORT_FILEINDEX_H
#define EXTERNAL_SORT_FILEINDEX_H

#include "FileWrapper.h"

#include <map>
#include <string>

namespace external_sort
{

class SortedFile {
public:
	void SaveLines(const RangeLines& lines);
	std::string GetFirst() const;
	bool Pop();

private:
	FileWrapper m_file;
	std::map<unsigned, std::pair<size_t, size_t> > m_offsets;
};

} // external_sort

#endif //EXTERNAL_SORT_FILEINDEX_H
