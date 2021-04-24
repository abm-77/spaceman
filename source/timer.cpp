#include <SDL2/SDL.h>
#include "timer.h"

void TimerStart(Timer* timer) {
    // Start the timer
    timer->started = true;

    // Unpause the timer
    timer->paused = false;

    // Get the current clock time
    timer->start_ticks = SDL_GetTicks();
    timer->paused_ticks = 0;
}

void TimerStop(Timer* timer) {
    //Stop the timer
    timer->started = false;

    //Unpause the timer
    timer->paused = false;

    //Clear tick variables
    timer->start_ticks = 0;
    timer->paused_ticks = 0;

}

void TimerPause(Timer* timer) {
    //If the timer is running and isn't already paused
    if( timer->started && !timer->paused )
    {
        //Pause the timer
        timer->paused = true;

        //Calculate the paused ticks
        timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
        timer->start_ticks = 0;
    }
}

void TimerUnpause(Timer* timer) {
    //If the timer is running and paused
    if( timer->started && timer->paused )
    {
        //Unpause the timer
        timer->paused = false;

        //Reset the starting ticks
        timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;

        //Reset the paused ticks
        timer->paused_ticks = 0;
    }   
}

u32 TimerGetTicks(Timer timer) {
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( timer.started )
    {
        //If the timer is paused
        if( timer.paused )
        {
            //Return the number of ticks when the timer was paused
            time = timer.paused_ticks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - timer.start_ticks;
        }
    }

    return time;
}

b32 TimerIsStarted(Timer timer) {
    //Timer is running and paused or unpaused
    return timer.started;
}

b32 TimerIsPaused(Timer timer) {
    //Timer is running and paused
    return timer.paused && timer.started;
}

f32 TimerGetElapsedTime(Timer timer) {
    if (timer.started) {
        return TimerGetTicks(timer) / 1000.0f;
    }

    return 0;
}