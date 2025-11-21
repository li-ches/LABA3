#include <benchmark/benchmark.h>
#include "../../../zad1/cpp/hash-table.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

// ==================== CUCKOO HASH TABLE BENCHMARKS ====================

static void BM_CuckooHashTable_Constructor(benchmark::State& state) {
    for (auto _ : state) {
        CuckooHashTable table(state.range(0));
        benchmark::DoNotOptimize(table);
    }
}
BENCHMARK(BM_CuckooHashTable_Constructor)->Arg(10)->Arg(50)->Arg(100);

static void BM_CuckooHashTable_Insert(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000);
    
    for (auto _ : state) {
        state.PauseTiming();
        CuckooHashTable table(10);
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            string key = "key_" + to_string(dist(gen));
            string value = "value_" + to_string(i);
            table.insert(key, value);
        }
    }
}
BENCHMARK(BM_CuckooHashTable_Insert)->Arg(10)->Arg(50)->Arg(100);

static void BM_CuckooHashTable_Search(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000);
    
    CuckooHashTable table;
    vector<string> keys;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(dist(gen));
        string value = "value_" + to_string(i);
        table.insert(key, value);
        keys.push_back(key);
    }
    
    for (auto _ : state) {
        for (const auto& key : keys) {
            auto result = table.search(key);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_CuckooHashTable_Search)->Arg(10)->Arg(50)->Arg(100);

static void BM_CuckooHashTable_Remove(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000);
    
    for (auto _ : state) {
        state.PauseTiming();
        CuckooHashTable table;
        vector<string> keys;
        for (int i = 0; i < state.range(0); ++i) {
            string key = "key_" + to_string(dist(gen));
            string value = "value_" + to_string(i);
            table.insert(key, value);
            keys.push_back(key);
        }
        state.ResumeTiming();
        
        for (const auto& key : keys) {
            table.remove(key);
        }
    }
}
BENCHMARK(BM_CuckooHashTable_Remove)->Arg(10)->Arg(50)->Arg(100);

static void BM_CuckooHashTable_SaveToFile(benchmark::State& state) {
    CuckooHashTable table;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        table.insert(key, value);
    }
    
    for (auto _ : state) {
        ofstream file("cuckoo_save.txt", ios::trunc);
        table.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_CuckooHashTable_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_CuckooHashTable_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("cuckoo_load.txt", ios::trunc);
    CuckooHashTable setupTable;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        setupTable.insert(key, value);
    }
    setupTable.saveToFile(setupFile);
    setupFile.close();
    
    for (auto _ : state) {
        CuckooHashTable table;
        ifstream file("cuckoo_load.txt");
        table.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(table);
    }
}
BENCHMARK(BM_CuckooHashTable_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_CuckooHashTable_SaveToBinaryFile(benchmark::State& state) {
    CuckooHashTable table;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        table.insert(key, value);
    }
    
    for (auto _ : state) {
        ofstream file("cuckoo_binary.bin", ios::binary | ios::trunc);
        table.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_CuckooHashTable_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_CuckooHashTable_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("cuckoo_binary_load.bin", ios::binary | ios::trunc);
    CuckooHashTable setupTable;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        setupTable.insert(key, value);
    }
    setupTable.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto _ : state) {
        CuckooHashTable table;
        ifstream file("cuckoo_binary_load.bin", ios::binary);
        table.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(table);
    }
}
BENCHMARK(BM_CuckooHashTable_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);

// ==================== DOUBLE HASH TABLE BENCHMARKS ====================

static void BM_DoubleHashTable_Constructor(benchmark::State& state) {
    for (auto _ : state) {
        DoubleHashTable table(state.range(0));
        benchmark::DoNotOptimize(table);
    }
}
BENCHMARK(BM_DoubleHashTable_Constructor)->Arg(10)->Arg(50)->Arg(100);

static void BM_DoubleHashTable_Insert(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000);
    
    for (auto _ : state) {
        state.PauseTiming();
        DoubleHashTable table(10);
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            string key = "key_" + to_string(dist(gen));
            string value = "value_" + to_string(i);
            table.insert(key, value);
        }
    }
}
BENCHMARK(BM_DoubleHashTable_Insert)->Arg(10)->Arg(50)->Arg(100);

static void BM_DoubleHashTable_Search(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000);
    
    DoubleHashTable table;
    vector<string> keys;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(dist(gen));
        string value = "value_" + to_string(i);
        table.insert(key, value);
        keys.push_back(key);
    }
    
    for (auto _ : state) {
        for (const auto& key : keys) {
            auto result = table.search(key);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_DoubleHashTable_Search)->Arg(10)->Arg(50)->Arg(100);

static void BM_DoubleHashTable_Remove(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000);
    
    for (auto _ : state) {
        state.PauseTiming();
        DoubleHashTable table;
        vector<string> keys;
        for (int i = 0; i < state.range(0); ++i) {
            string key = "key_" + to_string(dist(gen));
            string value = "value_" + to_string(i);
            table.insert(key, value);
            keys.push_back(key);
        }
        state.ResumeTiming();
        
        for (const auto& key : keys) {
            table.remove(key);
        }
    }
}
BENCHMARK(BM_DoubleHashTable_Remove)->Arg(10)->Arg(50)->Arg(100);

static void BM_DoubleHashTable_SaveToFile(benchmark::State& state) {
    DoubleHashTable table;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        table.insert(key, value);
    }
    
    for (auto _ : state) {
        ofstream file("double_save.txt", ios::trunc);
        table.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_DoubleHashTable_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_DoubleHashTable_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("double_load.txt", ios::trunc);
    DoubleHashTable setupTable;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        setupTable.insert(key, value);
    }
    setupTable.saveToFile(setupFile);
    setupFile.close();
    
    for (auto _ : state) {
        DoubleHashTable table;
        ifstream file("double_load.txt");
        table.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(table);
    }
}
BENCHMARK(BM_DoubleHashTable_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_DoubleHashTable_SaveToBinaryFile(benchmark::State& state) {
    DoubleHashTable table;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        table.insert(key, value);
    }
    
    for (auto _ : state) {
        ofstream file("double_binary.bin", ios::binary | ios::trunc);
        table.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_DoubleHashTable_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_DoubleHashTable_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("double_binary_load.bin", ios::binary | ios::trunc);
    DoubleHashTable setupTable;
    for (int i = 0; i < state.range(0); ++i) {
        string key = "key_" + to_string(i);
        string value = "value_" + to_string(i);
        setupTable.insert(key, value);
    }
    setupTable.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto _ : state) {
        DoubleHashTable table;
        ifstream file("double_binary_load.bin", ios::binary);
        table.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(table);
    }
}
BENCHMARK(BM_DoubleHashTable_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);

BENCHMARK_MAIN();