#include <benchmark/benchmark.h>
#include "../../../zad1/cpp/onelist.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

static void BM_OneList_Constructor(benchmark::State& state) {
    for (auto s : state) {
        OneList list;
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_OneList_Constructor);

static void BM_OneList_AddToHead(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            list.addToHead("danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_OneList_AddToHead)->Arg(10)->Arg(50)->Arg(100);

static void BM_OneList_AddToTail(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            list.addToTail("danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_OneList_AddToTail)->Arg(10)->Arg(50)->Arg(100);

static void BM_OneList_AddAfterValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.addAfterValue("bob_500", "danil_new");
    }
}
BENCHMARK(BM_OneList_AddAfterValue);

static void BM_OneList_AddBeforeValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.addBeforeValue("bob_500", "danil_new");
    }
}
BENCHMARK(BM_OneList_AddBeforeValue);

static void BM_OneList_RemoveFromHead(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        for (int i = 0; i < state.range(0); ++i) {
            list.addToTail("danil_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeFromHead();
    }
}
BENCHMARK(BM_OneList_RemoveFromHead)->Arg(10)->Arg(50)->Arg(100);

static void BM_OneList_RemoveFromTail(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        for (int i = 0; i < state.range(0); ++i) {
            list.addToTail("danil_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeFromTail();
    }
}
BENCHMARK(BM_OneList_RemoveFromTail)->Arg(10)->Arg(50)->Arg(100);

static void BM_OneList_RemoveAfterValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeAfterValue("bob_500");
    }
}
BENCHMARK(BM_OneList_RemoveAfterValue);

static void BM_OneList_RemoveBeforeValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeBeforeValue("bob_500");
    }
}
BENCHMARK(BM_OneList_RemoveBeforeValue);

static void BM_OneList_RemoveByValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        OneList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeByValue("bob_500");
    }
}
BENCHMARK(BM_OneList_RemoveByValue);

static void BM_OneList_GetElementByValue(benchmark::State& state) {
    OneList list;
    for (int i = 0; i < 1000; ++i) {
        list.addToTail("bob_" + to_string(i));
    }
    
    for (auto s : state) {
        auto result = list.getElementByValue("bob_" + to_string(state.range(0)));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_OneList_GetElementByValue)->Arg(0)->Arg(500)->Arg(999);

static void BM_OneList_FindByVal(benchmark::State& state) {
    OneList list;
    for (int i = 0; i < 1000; ++i) {
        list.addToTail("bob_" + to_string(i));
    }
    
    for (auto s : state) {
        auto result = list.findByVal("bob_" + to_string(state.range(0)));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_OneList_FindByVal)->Arg(0)->Arg(500)->Arg(999);

static void BM_OneList_SaveToFile(benchmark::State& state) {
    OneList list;
    for (int i = 0; i < state.range(0); ++i) {
        list.addToTail("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("onelist_save.txt", ios::trunc);
        list.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_OneList_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_OneList_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("onelist_load.txt", ios::trunc);
    for (int i = 0; i < state.range(0); ++i) {
        setupFile << "danil_" << i << " ";
    }
    setupFile.close();
    
    for (auto s : state) {
        OneList list;
        ifstream file("onelist_load.txt");
        list.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_OneList_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_OneList_SaveToBinaryFile(benchmark::State& state) {
    OneList list;
    for (int i = 0; i < state.range(0); ++i) {
        list.addToTail("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("onelist_binary.bin", ios::binary | ios::trunc);
        list.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_OneList_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_OneList_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("onelist_binary_load.bin", ios::binary | ios::trunc);
    OneList setupList;
    for (int i = 0; i < state.range(0); ++i) {
        setupList.addToTail("danil_" + to_string(i));
    }
    setupList.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto s : state) {
        OneList list;
        ifstream file("onelist_binary_load.bin", ios::binary);
        list.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_OneList_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);

BENCHMARK_MAIN();