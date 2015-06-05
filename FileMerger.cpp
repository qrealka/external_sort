#include "FileMerger.h"

namespace external_sort
{

FileMerger::FileMerger(const char* outFileName)
    : m_outFile(outFileName, false)
{

}

void FileMerger::Merge(const std::list<external_sort::FileWrapper> &list) {
    if (list.empty())
        return;

    if (list.size() == 1) {
        list.front().CopyFileTo(m_outFile);
        return;
    }
}

void FileMerger::Final() {
    m_outFile.Close();
}

}
