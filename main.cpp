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
		FileMerger merger(argv[2]);

        static_assert(MAX_SORTED_SIZE > 0, "Limit size of part of file canbnot be zero");
        static_assert(MAX_SORTED_SIZE <= 1024, "Limit size of part of file too big");
        const auto partSize = MAX_SORTED_SIZE * 1024L * 1024L;

        std::cout << "Start file " << argv[1] << " splitting...\n";
        splitter.Split(partSize);

        std::cout << "Start merge files to destination " << argv[2] << std::endl;
		splitter.Merge(merger);

        std::cout << "Finished\n";
        return 0;
    }
    catch(const std::exception& error) {
        std::cout << "Error: " << error.what() << std::flush;
        return -1;
    }
}