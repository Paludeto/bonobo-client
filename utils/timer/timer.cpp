#include "timer.h"

Timer::Timer() {
    // Gets starting value (nanoseconds since epoch) for _time1 and _time2 
    _time1 = std::chrono::high_resolution_clock::now();
    _time2 = std::chrono::high_resolution_clock::now();
}

void Timer::start() {
    // Gets starting value
    _time1 = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    // Gets last value
    _time2 = std::chrono::high_resolution_clock::now();
}

double Timer::getSeconds() {
    // Converts nanoseconds to seconds
    return (getNanoSeconds() / 1E9);
}

double Timer::getMiliSeconds() {
    // Converts nanoseconds to miliseconds
    return (getNanoSeconds() / 1E6);
}

double Timer::getMicroSeconds() {
    // Converts nanoseconds to microseconds
    return (getNanoSeconds() / 1E3);
}

double Timer::getNanoSeconds() {
    // "auto" gets variable type automatically. Cast converts subtraction (elapsed time) to chrono::nanoseconds.
    auto passedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(_time2 - _time1);
    return (passedTime.count());
}
