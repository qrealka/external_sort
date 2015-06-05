#include "FileMerger.h"
#include "FileSplitter.h"

namespace external_sort
{

FileMerger::FileMerger(const char* outFileName)
    : m_outFile(outFileName, false)
{

}

void FileMerger::Merge(FileSplitter &splitter) {
	m_outFile.Rewind();

	for (;;) {
		const auto minimum = splitter.FindNextMinimum();
		if (minimum.empty())
			break;

		m_outFile.Write(RangeConstChar(minimum.data(), minimum.data() + minimum.size()));
	}

	m_outFile.Close();
}


}
