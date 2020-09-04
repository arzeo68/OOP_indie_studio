//
// Created by alexis on 23/03/2020.
//

#ifndef OOP_ARCADE_2019_CLOCK_HPP
#define OOP_ARCADE_2019_CLOCK_HPP

#include <chrono>

class Clock
{
    public:
    Clock();

    ~Clock();

    void reset();

    float getElapsedTimeSinceLastReset();

    float getElapsedTimeSinceLastCall();

    private:
    std::chrono::system_clock::time_point breakpoint;
    std::chrono::system_clock::time_point lastCall;
};

#endif //OOP_ARCADE_2019_CLOCK_HPP
