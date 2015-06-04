#include "FileSplitter.h"
#include "FileMerger.h"
#include <iostream>


int main(int argc, const char* argv[]) {

    if (argc < 3) {
        std::cout << "Not enough parameters. Example usage: external_sort file_for_sort.txt sorted.txt\n";
        return -1;
    }

    //std::ios::sync_with_stdio(false);

    try {
        using external_sort::FileSplitter;
        using external_sort::FileWrapper;
        using external_sort::RangeLines;
        using external_sort::RangeConstChar;
        using external_sort::FileMerger;

        FileSplitter splitter(argv[1]);

        splitter.Split(MAX_SORTED_SIZE, [](FileWrapper& outFile, RangeLines& lines){
            std::sort(lines.begin(), lines.end());
            std::for_each(lines.begin(), lines.end(), [&outFile](RangeConstChar& range){
                outFile.Write(range);
            });
        });

        FileMerger merger(argv[2]);
        merger.Merge(splitter.GetSplitResults());
        merger.Final();

        return 0;
    }
    catch(const std::exception& error) {
        std::cout << "Error: " << error.what();
        return -1;
    }
}