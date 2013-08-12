#include "Timer.hpp"
#include <unistd.h>
#include <sys/time.h>

#define MICROSECONDS_IN_SECONDS 1000000

// Local function that returns number of miliseconds (1/1000)
// since the Epoch.
static unsigned int getTicks()
{
    struct timeval tmp;
    gettimeofday(&(tmp), NULL);

    // microseconds
    return (tmp.tv_usec / 1000) + (tmp.tv_sec * 1000);
    // suseconds_t
}

Timer::Timer()
{
    this->startMark  = 0;
    this->stopMark   = 0;
    this->pausedMark = 0;
    this->running    = false;
    this->paused     = false;
}
void Timer::start()
{
    if (this->running) return;

    this->startMark  = getTicks();
    this->stopMark   = 0;
    this->pausedMark = 0;
    this->running    = true;
    this->paused     = false;
}
void Timer::stop()
{
    if (!(this->running)) return;

    this->stopMark = getTicks();
    this->running  = false;
    this->paused   = false;
}
void Timer::restart()
{
    this->stop();
    this->start();
}
void Timer::pause()
{
    if (!running || paused) return;

    this->running    = false;
    this->paused     = true;
    this->pausedMark = (getTicks()) - (this->startMark);
}
void Timer::unpause()
{
    if (!paused || running) return;

    this->running = true;
    this->paused  = false;

    this->startMark  = (getTicks()) - (this->pausedMark);
    this->pausedMark = 0;
}
bool Timer::isRunning()
{
    return (this->running);
}
bool Timer::isPaused()
{
    return (this->paused);
}
int Timer::delta()
{
    if (this->isRunning())
        return (getTicks()) - (this->startMark);

    if (this->paused)
        return (this->pausedMark);

    // Something went wrong here
    if (this->startMark == 0)
        return 0;

    return (this->stopMark) - (this->startMark);
}
int Timer::delta_ms()
{
    return (this->delta() % 1000);
}
int Timer::delta_s()
{
    return (this->delta() / 1000);
}
float Timer::delta_sf()
{
    return (float)(this->delta() / 1000);
}

