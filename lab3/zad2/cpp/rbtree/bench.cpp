#include <benchmark/benchmark.h>
#include "../../../zad1/cpp/rbtree.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

static void BM_RBTree_Constructor(benchmark::State& state) {
    for (auto s : state) {
        RBTree tree;
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_RBTree_Constructor);

static void BM_RBTree_Insert(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000);
    
    for (auto s : state) {
        state.PauseTiming();
        RBTree tree;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            tree.insert(dist(gen));
        }
    }
}
BENCHMARK(BM_RBTree_Insert)->Arg(10)->Arg(50)->Arg(100);

static void BM_RBTree_Remove(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000);
    
    for (auto s : state) {
        state.PauseTiming();
        RBTree tree;
        set<int> uniqueValues;
        while (uniqueValues.size() < static_cast<size_t>(state.range(0))) {
            uniqueValues.insert(dist(gen));
        }

        vector<int> values(uniqueValues.begin(), uniqueValues.end());
        for (int val : values) {
            tree.insert(val);
        }
        state.ResumeTiming();
        
        int removeCount = min(static_cast<int>(values.size()), 5);
        for (int i = 0; i < removeCount; ++i) {
            tree.remove(values[i]);
        }
    }
}
BENCHMARK(BM_RBTree_Remove)->Arg(10)->Arg(50)->Arg(100);


static void BM_RBTree_Find(benchmark::State& state) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000);
    
    RBTree tree;
    vector<int> values;
    for (int i = 0; i < state.range(0); ++i) {
        int val = dist(gen);
        tree.insert(val);
        values.push_back(val);
    }
    
    for (auto s : state) {
        for (int i = 0; i < min(static_cast<int>(state.range(0)), 10); ++i) {
            auto result = tree.find(values[i]);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_RBTree_Find)->Arg(10)->Arg(50)->Arg(100);

static void BM_RBTree_Print(benchmark::State& state) {
    RBTree tree;
    for (int i = 0; i < state.range(0); ++i) {
        tree.insert(i + 1);
    }
    
    for (auto s : state) {
        stringstream ss;
        auto old_buf = cout.rdbuf(ss.rdbuf());
        tree.print();
        cout.rdbuf(old_buf);
        benchmark::DoNotOptimize(ss);
    }
}
BENCHMARK(BM_RBTree_Print)->Arg(10)->Arg(50)->Arg(100);

static void BM_RBTree_SaveToFile(benchmark::State& state) {
    RBTree tree;
    for (int i = 0; i < state.range(0); ++i) {
        tree.insert(i + 1);
    }
    
    for (auto s : state) {
        ofstream file("rbtree_save.txt", ios::trunc);
        tree.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_RBTree_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_RBTree_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("rbtree_load.txt", ios::trunc);
    RBTree setupTree;
    for (int i = 0; i < state.range(0); ++i) {
        setupTree.insert(i + 1);
    }
    setupTree.saveToFile(setupFile);
    setupFile.close();
    
    for (auto s : state) {
        RBTree tree;
        ifstream file("rbtree_load.txt");
        tree.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_RBTree_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_RBTree_SaveToBinaryFile(benchmark::State& state) {
    RBTree tree;
    for (int i = 0; i < state.range(0); ++i) {
        tree.insert(i + 1);
    }
    
    for (auto s : state) {
        ofstream file("rbtree_binary.bin", ios::binary | ios::trunc);
        tree.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_RBTree_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_RBTree_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("rbtree_binary_load.bin", ios::binary | ios::trunc);
    RBTree setupTree;
    for (int i = 0; i < state.range(0); ++i) {
        setupTree.insert(i + 1);
    }
    setupTree.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto s : state) {
        RBTree tree;
        ifstream file("rbtree_binary_load.bin", ios::binary);
        tree.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_RBTree_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_RBTree_Destructor(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        RBTree* tree = new RBTree();
        for (int i = 0; i < state.range(0); ++i) {
            tree->insert(i + 1);
        }
        state.ResumeTiming();
        
        delete tree;
    }
}

BENCHMARK_MAIN();