cmake_minimum_required (VERSION 2.6)
project (TopTreeAnalysisBase)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
#subdirs ()

enable_testing()
subdirs (test)
