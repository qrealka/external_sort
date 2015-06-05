#include "FileSplitter.h"
#include "FileMerger.h"
#include <iostream>

using namespace external_sort;

int main(int argc, const char* argv[]) {

    if (argc < 3) {
        std::cout << "Not enough parameters. Example usage: external_sort file_for_sort.txt sorted.txt\n";
        return -1;
    }

    try {
        FileSplitter splitter(argv[1]);

        const int64_t partSize = MAX_SORTED_SIZE * 1024LL * 1024LL * 1024LL;
        static_assert(MAX_SORTED_SIZE > 0, "Limit size of part of file canbnot be zero");

        std::cout << "Start file " << argv[1] << " splitting...\n";
        splitter.Split(partSize, [](FileWrapper& outFile, RangeLines& lines){
            std::sort(lines.begin(), lines.end());
            std::for_each(lines.begin(), lines.end(), [&outFile](RangeConstChar& range){
                outFile.Write(range);
            });
        });

        std::cout << "Start merge files to destination " << argv[2] << std::endl;
        FileMerger merger(argv[2]);
        merger.Merge(splitter.GetSplitResults());
        merger.Final();

        std::cout << "Finished\n";
        return 0;
    }
    catch(const std::exception& error) {
        std::cout << "Error: " << error.what() << std::flush;
        return -1;
    }
}