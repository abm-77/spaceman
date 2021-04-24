#pragma once

#include "language_layer.h"

struct Timer {
    b32 paused = false;
    b32 started = false; 

    u32 paused_ticks = 0;
    u32 start_ticks = 0;
};

void TimerStart(Timer* timer);
void TimerStart(Timer* timer);
void TimerStop(Timer* timer);
void TimerPause(Timer* timer);
void TimerUnpause(Timer* timer);

u32 TimerGetTicks(Timer timer);
f32 TimerGetElapsedTime(Timer timer);
b32 TimerIsStarted(Timer timer);
b32 TimerIsPaused(Timer timer);