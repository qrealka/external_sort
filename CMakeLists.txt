cmake_minimum_required(VERSION 3.2)
project(external_sort)

# MAX_SORTED_SIZE - maximum size (in GB) of part of input file fol sorting
# FILE_CHUNK_SIZE - size (in MB) of chunk data readed from input file
# MAX_STRING_SIZE - maximum size (in KB) of strings

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# specify maximum size part of input file in GB
if (NOT DEFINED MAX_SORTED_SIZE)
    add_definitions(-DMAX_SORTED_SIZE=16384)
else()
    add_definitions(-DMAX_SORTED_SIZE=${MAX_SORTED_SIZE})
endif()

# file chunk size for reading in MB
if (NOT DEFINED FILE_CHUNK_SIZE)
    add_definitions(-DFILE_CHUNK_SIZE=8192)
else()
    add_definitions(-DFILE_CHUNK_SIZE=${FILE_CHUNK_SIZE})
endif()

# limit std:string size in KB
if (NOT DEFINED MAX_STRING_SIZE)
    add_definitions(-DMAX_STRING_SIZE=512)
else()
    add_definitions(-DMAX_STRING_SIZE=${MAX_STRING_SIZE})
endif()

set(SOURCE_FILES main.cpp)
add_executable(external_sort ${SOURCE_FILES} FileSplitter.cpp FileSplitter.h ThrowError.h Range.h FileWrapper.cpp FileWrapper.h FileMerger.cpp FileMerger.h)