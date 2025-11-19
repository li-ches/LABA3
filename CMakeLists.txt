cmake_minimum_required(VERSION 3.16)
project(LABS3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "=== Конфигурация LABS3 ===")

include(FetchContent)

# ==========================================
# 1. GoogleTest (правильное подключение)
# ==========================================
message(STATUS "Загружаем GoogleTest…")

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.14.0.zip
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# ==========================================
# 2. Библиотека структур данных
# ==========================================
set(LABS3_SOURCES
    arr.cpp
    dList.cpp
    list.cpp
    stack.cpp
    queue.cpp
    hash.cpp
    compl.cpp
)

foreach(SRC ${LABS3_SOURCES})
    message(STATUS "Добавляем исходник структуры: ${SRC}")
endforeach()

add_library(labs3_lib ${LABS3_SOURCES})
target_include_directories(labs3_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Создана библиотека labs3_lib")

# ==========================================
# 3. Тесты
# ==========================================
add_subdirectory(tests)

# ==========================================
# 4. Отдельный бинарник serialization_runner
# ==========================================
add_executable(serialization_runner serialization_runner.cpp)
target_link_libraries(serialization_runner PRIVATE labs3_lib)
message(STATUS "Создан бинарник serialization_runner")
