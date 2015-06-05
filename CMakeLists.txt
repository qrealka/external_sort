cmake_minimum_required(VERSION 3.1)
project(external_sort)

# MAX_SORTED_SIZE       - maximum size (in GB) of part of input file fol sorting
# MAX_STRING_SIZE       - maximum size (in KB) of strings
# MAX_INPUT_FILE_SIZE   - maximum input file size in GB

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# specify maximum inpu file size in GB
if (NOT DEFINED MAX_INPUT_FILE_SIZE)
    add_definitions(-DMAX_INPUT_FILE_SIZE=200)
else()
    add_definitions(-DMAX_INPUT_FILE_SIZE=${MAX_INPUT_FILE_SIZE})
endif()

# specify maximum size part of input file in GB
if (NOT DEFINED MAX_SORTED_SIZE)
    add_definitions(-DMAX_SORTED_SIZE=1)
else()
    add_definitions(-DMAX_SORTED_SIZE=${MAX_SORTED_SIZE})
endif()

# limit std:string size in KB
if (NOT DEFINED MAX_STRING_SIZE)
    add_definitions(-DMAX_STRING_SIZE=64)
else()
    add_definitions(-DMAX_STRING_SIZE=${MAX_STRING_SIZE})
endif()

aux_source_directory(mphf SRC_HASH)
set(SOURCE_FILES main.cpp FileSplitter.cpp FileSplitter.h ThrowError.h Range.h FileWrapper.cpp FileWrapper.h FileMerger.cpp FileMerger.h SortedFile.cpp SortedFile.h Chm.hpp Chm.cpp  ${SRC_HASH})
add_executable(external_sort ${SOURCE_FILES})