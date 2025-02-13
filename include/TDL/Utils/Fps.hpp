
#ifndef FPS_HPP
    #define FPS_HPP

#include <iostream>
#include <chrono>

class FPSCounter {
public:
    FPSCounter() : frameCount(0), startTime(std::chrono::high_resolution_clock::now()) {}

    void frameRendered() {
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;

        if (elapsed.count() >= 1.0) {
            fps = frameCount / elapsed.count();
            frameCount = 0;
            startTime = currentTime;
            std::cerr << "FPS: " << fps << std::endl;
        }
    }

private:
    int frameCount;
    double fps;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

#endif //FPS_HPP
