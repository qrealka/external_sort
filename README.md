# External Sort
Simple external sort huge file implementation.

# Compilers:
* VisualStudio 2010+
* MinGW 3.2 (gcc 4.8.1)
* Linux GCC 4.8.1+

# Build instructions
1. mkdir build && cd build
2. cmake .. or cmake -G"Visual Studio <version>" ..
3. use generated files

# Debug hints
You can debug this algo on small files too by using define DEBUG_MERGE. For example:
1. Generate a test file with lines of 6 characters long
2. Rebuild project with define 'DEBUG_MERGE=6' (make -DDEBUG_MERGE=6)
3. Start debug

