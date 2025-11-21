#include <benchmark/benchmark.h>
#include "../../../zad1/cpp/queue.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

static void BM_Queue_Constructor(benchmark::State& state) {
    for (auto s : state) {
        Queue queue;
        benchmark::DoNotOptimize(queue);
    }
}
BENCHMARK(BM_Queue_Constructor);

static void BM_Queue_Enqueue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        Queue queue;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            queue.enqueue("danil_" + to_string(i));
        }
    }
}
BENCHMARK(BM_Queue_Enqueue)->Arg(10)->Arg(50)->Arg(100);

static void BM_Queue_Dequeue(benchmark::State& state) {
    for (auto s : state) {
        state.PauseTiming();
        Queue queue;
        for (int i = 0; i < state.range(0); ++i) {
            queue.enqueue("danil_" + to_string(i));
        }
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); ++i) {
            queue.dequeue();
        }
    }
}
BENCHMARK(BM_Queue_Dequeue)->Arg(10)->Arg(50)->Arg(100);

static void BM_Queue_SaveToFile(benchmark::State& state) {
    Queue queue;
    for (int i = 0; i < state.range(0); ++i) {
        queue.enqueue("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("queue_save.txt", ios::trunc);
        queue.saveToFile(file);
        file.close();
    }
}
BENCHMARK(BM_Queue_SaveToFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_Queue_LoadFromFile(benchmark::State& state) {
    ofstream setupFile("queue_load.txt", ios::trunc);
    for (int i = 0; i < state.range(0); ++i) {
        setupFile << "danil_" << i << " ";
    }
    setupFile.close();
    
    for (auto s : state) {
        Queue queue;
        ifstream file("queue_load.txt");
        queue.loadFromFile(file);
        file.close();
        benchmark::DoNotOptimize(queue);
    }
}
BENCHMARK(BM_Queue_LoadFromFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_Queue_SaveToBinaryFile(benchmark::State& state) {
    Queue queue;
    for (int i = 0; i < state.range(0); ++i) {
        queue.enqueue("danil_" + to_string(i));
    }
    
    for (auto s : state) {
        ofstream file("queue_binary.bin", ios::binary | ios::trunc);
        queue.saveToBinaryFile(file);
        file.close();
    }
}
BENCHMARK(BM_Queue_SaveToBinaryFile)->Arg(10)->Arg(50)->Arg(100);

static void BM_Queue_LoadFromBinaryFile(benchmark::State& state) {
    ofstream setupFile("queue_binary_load.bin", ios::binary | ios::trunc);
    Queue setupQueue;
    for (int i = 0; i < state.range(0); ++i) {
        setupQueue.enqueue("danil_" + to_string(i));
    }
    setupQueue.saveToBinaryFile(setupFile);
    setupFile.close();
    
    for (auto s : state) {
        Queue queue;
        ifstream file("queue_binary_load.bin", ios::binary);
        queue.loadFromBinaryFile(file);
        file.close();
        benchmark::DoNotOptimize(queue);
    }
}
BENCHMARK(BM_Queue_LoadFromBinaryFile)->Arg(10)->Arg(50)->Arg(100);

BENCHMARK_MAIN();