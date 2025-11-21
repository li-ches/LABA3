#include <benchmark/benchmark.h>
#include "../../../zad1/cpp/stack.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

static void BM_Stack_Constructor(benchmark::State& state) {
    for (auto s : state) {
        Stack stack;
        benchmark::DoNotOptimize(stack);
    }
}
BENCHMARK(BM_Stack_Constructor);

static void BM_Stack_Push(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        Stack stack;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            stack.push("danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_Stack_Push)->Arg(10)->Arg(50)->Arg(100);

static void BM_Stack_Pop(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        Stack stack;
        for (int i = 0; i < state.range(0); ++i) {
            stack.push("danil_" + to_string(i));
        }
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            stack.pop();
        }
    }
}
BENCHMARK(BM_Stack_Pop)->Arg(10)->Arg(50)->Arg(100);

static void BM_Stack_SaveToFile(benchmark::State& state) {
    Stack stack;
    for (int i = 0; i < state.range(0); ++i) {
        stack.push("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("stack_save.txt", ios::trunc);
        stack.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_Stack_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_Stack_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("stack_load.txt", ios::trunc);
    for (int i = 0; i < state.range(0); ++i) {
        setupFile << "danil_" << i << " ";
    }
    setupFile.close();
    
    for (auto s : state) {
        Stack stack;
        ifstream file("stack_load.txt");
        stack.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(stack);
    }
}
BENCHMARK(BM_Stack_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_Stack_SaveToBinaryFile(benchmark::State& state) {
    Stack stack;
    for (int i = 0; i < state.range(0); ++i) {
        stack.push("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("stack_binary.bin", ios::binary | ios::trunc);
        stack.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_Stack_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_Stack_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("stack_binary_load.bin", ios::binary | ios::trunc);
    Stack setupStack;
    for (int i = 0; i < state.range(0); ++i) {
        setupStack.push("danil_" + to_string(i));
    }
    setupStack.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto s : state) {
        Stack stack;
        ifstream file("stack_binary_load.bin", ios::binary);
        stack.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(stack);
    }
}
BENCHMARK(BM_Stack_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);

BENCHMARK_MAIN();