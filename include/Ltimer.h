#ifndef LTIMER_H__
#define LTIMER_H__

#include <SDL.h>
//----------------------------------------------------------------------------------------------------------------------
/// @file Ltimer.h
/// @class LTimer
/// @brief This class is timer that uses SDL to manage time
/// Modified from -:
/// Lazyfoo.net. (2014). Lazy Foo' Productions. [online]
/// Available at: http://lazyfoo.net/SDL_tutorials/lesson15/ [Accessed 8 Dec. 2015].
//----------------------------------------------------------------------------------------------------------------------
class LTimer
{
  public:
    //Initializes variables
    LTimer();
    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();
    void reset();
    //Gets the timer's time
    Uint32 getTicks();
    //Checks the status of the timer
    bool isStarted();
    bool isPaused();
  private:
    //The clock time when the timer started
    Uint32 m_startTicks;
    //The ticks stored when the timer was paused
    Uint32 m_pausedTicks;
    //The timer status
    bool m_paused;
    bool m_started;
};

#endif // LTIMER_H
