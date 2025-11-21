#include <benchmark/benchmark.h>
#include "../../../zad1/cpp/twolist.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

static void BM_TwoList_Constructor(benchmark::State& state) {
    for (auto s : state) {
        TwoList list;
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_TwoList_Constructor);

static void BM_TwoList_AddToHead(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            list.addToHead("danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_TwoList_AddToHead)->Arg(10)->Arg(50)->Arg(100);


static void BM_TwoList_AddToTail(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            list.addToTail("danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_TwoList_AddToTail)->Arg(10)->Arg(50)->Arg(100);

static void BM_TwoList_AddAfterValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.addAfterValue("bob_500", "danil_new");
    }
}
BENCHMARK(BM_TwoList_AddAfterValue);

static void BM_TwoList_AddBeforeValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.addBeforeValue("bob_500", "danil_new");
    }
}
BENCHMARK(BM_TwoList_AddBeforeValue);

static void BM_TwoList_RemoveFromHead(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        for (int i = 0; i < state.range(0); ++i) {
            list.addToTail("danil_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeFromHead();
    }
}
BENCHMARK(BM_TwoList_RemoveFromHead)->Arg(10)->Arg(50)->Arg(100);

static void BM_TwoList_RemoveFromTail(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        for (int i = 0; i < state.range(0); ++i) {
            list.addToTail("danil_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeFromTail();
    }
}
BENCHMARK(BM_TwoList_RemoveFromTail)->Arg(10)->Arg(50)->Arg(100);

static void BM_TwoList_RemoveAfterValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeAfterValue("bob_500");
    }
}
BENCHMARK(BM_TwoList_RemoveAfterValue);

static void BM_TwoList_RemoveBeforeValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeBeforeValue("bob_500");
    }
}
BENCHMARK(BM_TwoList_RemoveBeforeValue);

static void BM_TwoList_RemoveByValue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        TwoList list;
        for (int i = 0; i < 1000; ++i) {
            list.addToTail("bob_" + to_string(i));
        }
        state.ResumeTiming();
        
        list.removeByValue("bob_500");
    }
}
BENCHMARK(BM_TwoList_RemoveByValue);

static void BM_TwoList_GetElementByValue(benchmark::State& state) {
    TwoList list;
    for (int i = 0; i < 1000; ++i) {
        list.addToTail("bob_" + to_string(i));
    }
    
    for (auto s : state) {
        auto result = list.getElementByValue("bob_" + to_string(state.range(0)));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_TwoList_GetElementByValue)->Arg(0)->Arg(500)->Arg(999);

static void BM_TwoList_FindByVal(benchmark::State& state) {
    TwoList list;
    for (int i = 0; i < 1000; ++i) {
        list.addToTail("bob_" + to_string(i));
    }
    
    for (auto s : state) {
        auto result = list.findByVal("bob_" + to_string(state.range(0)));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_TwoList_FindByVal)->Arg(0)->Arg(500)->Arg(999);

static void BM_TwoList_SaveToFile(benchmark::State& state) {
    TwoList list;
    for (int i = 0; i < state.range(0); ++i) {
        list.addToTail("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("twolist_save.txt", ios::trunc);
        list.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_TwoList_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_TwoList_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("twolist_load.txt", ios::trunc);
    for (int i = 0; i < state.range(0); ++i) {
        setupFile << "danil_" << i << " ";
    }
    setupFile.close();
    
    for (auto s : state) {
        TwoList list;
        ifstream file("twolist_load.txt");
        list.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_TwoList_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_TwoList_SaveToBinaryFile(benchmark::State& state) {
    TwoList list;
    for (int i = 0; i < state.range(0); ++i) {
        list.addToTail("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("twolist_binary.bin", ios::binary | ios::trunc);
        list.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_TwoList_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_TwoList_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("twolist_binary_load.bin", ios::binary | ios::trunc);
    TwoList setupList;
    for (int i = 0; i < state.range(0); ++i) {
        setupList.addToTail("danil_" + to_string(i));
    }
    setupList.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto s : state) {
        TwoList list;
        ifstream file("twolist_binary_load.bin", ios::binary);
        list.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(list);
    }
}
BENCHMARK(BM_TwoList_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);

BENCHMARK_MAIN();