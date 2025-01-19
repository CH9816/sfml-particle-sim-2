#pragma once
#include <random>


class RandomGen {
private:
    std::mt19937 mt;
    std::uniform_real_distribution<double> range;
public:
    RandomGen(unsigned int seed = 0) {

        if (seed == 0) {
            std::random_device rand;
            mt = std::mt19937(rand());
        }
        else {
            mt = std::mt19937(seed);
        }

        range = std::uniform_real_distribution<double>(0, 1);
    }
    double get() {
        return range(mt);
    }

    int randint(int start, int end) {
        return start + (end - start + 1) * get();
    }
};