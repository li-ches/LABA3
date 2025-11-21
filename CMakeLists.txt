cmake_minimum_required(VERSION 3.16)

project(DataStructuresProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Настройка Линтера ---
find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    set(LINT_COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR})
else()
    message(WARNING "clang-tidy not found. Linting target will not be available.")
endif()

# --- Библиотеки (GTest & GBenchmark) ---
include(FetchContent)

# Google Test
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Google Benchmark
FetchContent_Declare(
  googlebenchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googlebenchmark)

# 1. Библиотека структур данных (файлы лежат в корне)
add_library(DataStructures
    arr.cpp arr.h
    list.cpp list.h
    dlist.cpp dlist.h
    stack.cpp stack.h
    queue.cpp queue.h
    compl.cpp compl.h
    hash.cpp hash.h
    serialize.h
)
target_include_directories(DataStructures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(LINT_COMMAND)
    set_target_properties(DataStructures PROPERTIES CXX_CLANG_TIDY ${LINT_COMMAND})
endif()

# 2. Главная программа (CLI)
add_executable(prg main.cpp)
target_link_libraries(prg PRIVATE DataStructures)

# 3. Тест сериализации (ЛЕЖИТ В ПАПКЕ tests)
add_executable(serialization_test tests/serialization_test.cpp)
target_link_libraries(serialization_test PRIVATE DataStructures)

# --- Google Tests ---
enable_testing()
include(GoogleTest)

# Указываем правильный путь: tests/
set(TEST_SOURCES
    tests/arr_test.cpp
    tests/compl_test.cpp
    tests/dlist_test.cpp
    tests/hash_test.cpp
    tests/list_test.cpp
    tests/queue_test.cpp
    tests/stack_test.cpp
)

foreach(TFILE ${TEST_SOURCES})
    get_filename_component(TNAME ${TFILE} NAME_WE)
    
    add_executable(${TNAME} ${TFILE})
    target_link_libraries(${TNAME}
    PRIVATE
        DataStructures
        gtest
        gtest_main
    )
    
    gtest_discover_tests(${TNAME})
endforeach()