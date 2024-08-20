#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:
    Timer();
    void start(unsigned long duration);
    void stop();
    bool isRunning() const;
    bool hasExpired() const;
    unsigned long getRemainingTime() const;

private:
    unsigned long startTime;
    unsigned long duration;
    bool running;
};

#endif // TIMER_H
