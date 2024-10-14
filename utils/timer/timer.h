#ifndef TIMER_H
#define TIMER_H

#include <chrono>

/**
 * @brief Class used to determine loop runtime by using std::chrono.
 * 
 */
class Timer
{
public:
    Timer();

    // Timer control
    void start();
    void stop();

    // Getters
    double getSeconds();
    double getMiliSeconds();
    double getMicroSeconds();
    double getNanoSeconds();

private:
    std::chrono::high_resolution_clock::time_point _time1;
    std::chrono::high_resolution_clock::time_point _time2;
};

#endif // TIMER_H
