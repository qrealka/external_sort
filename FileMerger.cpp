#include "FileMerger.h"
#include "FileSplitter.h"

namespace external_sort
{

FileMerger::FileMerger(const char* outFileName)
    : m_outFile(outFileName, false)
{

}

void FileMerger::Merge(const FileSplitter &splitter) {
	m_outFile.Rewind();

	for (;;) {
		const auto& minimum = splitter.FindNextMinimum();
		if (minimum.empty())
			break;

		m_outFile.Write(minimum);
	}

	m_outFile.Close();
}


}
