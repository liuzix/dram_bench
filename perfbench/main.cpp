#include <iostream>
#include <random>
#include <ctime>
#include <thread>
#include <cstdlib>
#include <vector>
#include <future>

using namespace std;

#define TOTAL_ITERS 10000000

float doOneThread(size_t nIters) {
    default_random_engine generator;
    normal_distribution<float> distribution(8.0f, 3.0f);

    vector<void*> chunks;
    chunks.reserve(nIters);

    clock_t begin = clock();

    for (int i = 0; i < nIters; i++) {
        float pow = distribution(generator);
        if (pow >= 12 || pow <= 0) pow = 8;
        size_t sizeChunk = static_cast<size_t>(2 << (size_t)pow);
        void* chunk = malloc(sizeChunk);
        if (!chunk) {
            cerr << "Malloc failed with size " << sizeChunk << endl;
            exit(EXIT_FAILURE);
        }
        chunks.push_back(chunk);
    }

    for (void* ptr: chunks) {
        free(ptr);
    }

    clock_t end = clock();

    return float(end - begin) / CLOCKS_PER_SEC;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: perfbench [NUM THREADS]" << endl;
        return -1;
    }

    auto nThreads = static_cast<size_t>(atoi(argv[1]));
    vector<future<float>> futures;
    for (int i = 0; i < nThreads; i++) {
        auto f = async(launch::async, doOneThread, TOTAL_ITERS / nThreads);
        futures.push_back(move(f));
    }

    float time = 0.0;
    for (auto& f: futures) {
        f.wait();
        time += f.get();
    }

    cout << "Average finishing time " << time / (float)nThreads << endl;


    return 0;
}