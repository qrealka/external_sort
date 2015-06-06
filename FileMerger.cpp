#include "FileMerger.h"
#include "FileSplitter.h"

#include <set>

namespace external_sort
{


FileMerger::FileMerger(const char* outFileName)
    : m_outFile(outFileName, false)
{

}

void FileMerger::Merge(FileSplitter::SplitIterator begin, FileSplitter::SplitIterator end) {
	m_outFile.Rewind();
	
	struct Less {
		bool operator()(const SortedFile* l, const SortedFile* r) {
			return l->GetFirst() < r->GetFirst();
		}
	};

	std::set<SortedFile*, Less> splits;
	for (; begin != end; ++begin) {
		splits.insert(&*begin);
	}

	while (!splits.empty()) {
		SortedFile* minimum = *splits.cbegin();

		const auto& top = minimum->GetFirst();
		m_outFile.Write(RangeConstChar(top.data(), top.data() + top.size()));

		if (!minimum->Pop())
			splits.erase(splits.cbegin());
	}

	m_outFile.Close();
}


}
