# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/chesnokova/LABA3/build/_deps/googlebenchmark-src"
  "/home/chesnokova/LABA3/build/_deps/googlebenchmark-build"
  "/home/chesnokova/LABA3/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix"
  "/home/chesnokova/LABA3/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/tmp"
  "/home/chesnokova/LABA3/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
  "/home/chesnokova/LABA3/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src"
  "/home/chesnokova/LABA3/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/chesnokova/LABA3/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/chesnokova/LABA3/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
