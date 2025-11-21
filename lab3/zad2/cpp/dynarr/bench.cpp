#include <benchmark/benchmark.h>
#include "../../../zad1/cpp/dynarr.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
using namespace std;
static void BM_DynArr_Constructor(benchmark::State& state) {
    for (auto s : state) {
        DynArr arr(state.range(0));
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_DynArr_Constructor)->Arg(10)->Arg(100)->Arg(1000);

static void BM_DynArr_AddToEnd(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        DynArr arr(10);
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            arr.addToEnd("danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_DynArr_AddToEnd)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_AddAtIndex(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        DynArr arr(10);
        for (int i = 0; i < 100; ++i) {
            arr.addToEnd("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            arr.addAtIndex(i % (arr.getLength() + 1), "danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_DynArr_AddAtIndex)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_GetElement(benchmark::State& state) {
    DynArr arr;
    for (int i = 0; i < state.range(0); ++i) {
        arr.addToEnd("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        for (int i = 0; i < arr.getLength(); ++i) {
            auto result = arr.getElement(i);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_DynArr_GetElement)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_ReplaceElement(benchmark::State& state) {
    DynArr arr;
    for (int i = 0; i < state.range(0); ++i) {
        arr.addToEnd("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        for (int i = 0; i < arr.getLength(); ++i) {
            arr.replaceElement(i, "danil_replaced_" + to_string(i));
        }
    }
}
BENCHMARK(BM_DynArr_ReplaceElement)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_RemoveAtIndex(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        DynArr arr;
        for (int i = 0; i < state.range(0); ++i) {
            arr.addToEnd("danil_" + to_string(i));
        }
        state.ResumeTiming();
        
        while (arr.getLength() > 0) {
            arr.removeAtIndex(0);
        }
    }
}
BENCHMARK(BM_DynArr_RemoveAtIndex)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_PrintArr(benchmark::State& state) {
    DynArr arr;
    for (int i = 0; i < state.range(0); ++i) {
        arr.addToEnd("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        stringstream ss;
        auto old_buf = cout.rdbuf(ss.rdbuf());
        arr.printArr();
        cout.rdbuf(old_buf);
        benchmark::DoNotOptimize(ss);
    }
}
BENCHMARK(BM_DynArr_PrintArr)->Arg(10)->Arg(100)->Arg(1000);

static void BM_DynArr_SaveToFile(benchmark::State& state) {
    DynArr arr;
    for (int i = 0; i < state.range(0); ++i) {
        arr.addToEnd("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("test_save.txt", ios::trunc);
        arr.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_DynArr_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("test_load.txt", ios::trunc);
    for (int i = 0; i < state.range(0); ++i) {
        setupFile << "danil_" << i << " ";
    }
    setupFile.close();
    
    for (auto s : state) {
        DynArr arr;
        ifstream file("test_load.txt");
        arr.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_DynArr_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_SaveToBinaryFile(benchmark::State& state) {
    DynArr arr;
    for (int i = 0; i < state.range(0); ++i) {
        arr.addToEnd("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("test_binary.bin", ios::binary | ios::trunc);
        arr.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_DynArr_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_DynArr_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("test_binary_load.bin", ios::binary | ios::trunc);
    DynArr setupArr;
    for (int i = 0; i < state.range(0); ++i) {
        setupArr.addToEnd("danil_" + to_string(i));
    }
    setupArr.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto s : state) {
        DynArr arr;
        ifstream file("test_binary_load.bin", ios::binary);
        arr.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_DynArr_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);


BENCHMARK_MAIN();