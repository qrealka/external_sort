#include "FileSplitter.h"
#include "ThrowError.h"

namespace external_sort
{

FileSplitter::FileSplitter(const std::string &inputFileName)
        : m_file(inputFileName, true)
        , m_chunk(FILE_CHUNK_SIZE)
{

}

void FileSplitter::Split(int splitSize, OnSplitCallback onSplit) {

}

const std::list<FileWrapper>& FileSplitter::GetSplitResults() const {
    return m_parts;
}

} // external_sort
