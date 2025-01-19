#pragma once
#include <chrono>

struct Timer {

    std::chrono::high_resolution_clock::time_point startTime;

    double start() {
        double val = get();
        startTime = std::chrono::high_resolution_clock::now();
        return val;
    }

    Timer(bool startBool = true) {
        if (startBool)
            start();
    }
    
    double get(int powerOf10 = 0) const {
        return
            std::chrono::duration_cast
                <std::chrono::nanoseconds>
                    (std::chrono::high_resolution_clock::now() - startTime).count()
                        / pow(10, 9 + powerOf10);
    }

};