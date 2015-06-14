#ifndef EXTERNAL_SORT_FILEMERGER_H
#define EXTERNAL_SORT_FILEMERGER_H

#include "FileWrapper.h"
#include <set>
#include <iterator>

namespace external_sort
{

template<typename Iterator>
void MergeSortedTo(Iterator begin, Iterator end, FileWrapper& outFile)
{
	typedef typename std::iterator_traits<Iterator>::pointer SortedFilePtr;

	outFile.Rewind();
	struct Less {
		bool operator()(const SortedFilePtr l, const SortedFilePtr r) {
			return l->GetFirst() < r->GetFirst();
		}
	};

	std::set<SortedFilePtr, Less> splits;
	for (; begin != end; ++begin) {
		splits.insert(&*begin);
	}

	while (!splits.empty()) {
		const auto minimum = *splits.cbegin();

		const auto& top = minimum->GetFirst();
		outFile.Write(RangeConstChar(top.data(), top.data() + top.size()));
		
		splits.erase(splits.cbegin());
		if (minimum->Pop())
			splits.insert(minimum);
	}

	outFile.Close();
}

}

#endif //EXTERNAL_SORT_FILEMERGER_H
