#include "Timer.h"

Timer::Timer() : startTime(0), duration(0), running(false) {}

void Timer::start(unsigned long duration) {
    this->startTime = millis();
    this->duration = duration;
    this->running = true;
}

void Timer::stop() {
    running = false;
}

bool Timer::isRunning() const {
    return running;
}

bool Timer::hasExpired() const {
    return running && (millis() - startTime >= duration);
}

unsigned long Timer::getRemainingTime() const {
    if (!running) return 0;
    unsigned long elapsed = millis() - startTime;
    return (elapsed >= duration) ? 0 : (duration - elapsed);
}
