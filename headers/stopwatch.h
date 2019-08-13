#pragma once
#include <chrono>

namespace SALIB {
    class StopWatch {
    private:
        std::chrono::high_resolution_clock::time_point start;

    public:
        StopWatch() : start(std::chrono::high_resolution_clock::now()) {}

        double get_duration() const {
            std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start).count();
        }
    };
}