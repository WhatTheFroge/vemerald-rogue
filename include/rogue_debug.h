#ifndef ROGUE_DEBUG_H
#define ROGUE_DEBUG_H

// Comment features in if want to use them

// Memory Stomp Tracking:
// - Helper to narrow down where a memory stomp is happening
// Usage:
// MEMORY_STOMP_TRACKING_SET_TARGET - Pass in the memory address of the target (Be aware of deallocs)
// MEMORY_STOMP_TRACKING_POLL - Place around the code to narrow down where the stomp is happening
//
//
// Frame Timers:
// - Help profiling specific areas of the frame to figure out where the slowdowns are
// Usage:
// (In game) Press L + A to print to mGBA log the next frame's timers
// FOR_EACH_TIMER - Add new timers to here
// START_TIMER - Start timer of specific id
// STOP_TIMER - Start timer of specific id
//

// (uncomment to enable)
#ifdef ROGUE_DEBUG
//#define DEBUG_FEATURE_MEMORY_STOMP_TRACKING
//#define DEBUG_FEATURE_FRAME_TIMERS
#endif


void RogueDebug_MainInit(void);
void RogueDebug_MainCB(void);

void RogueDebug_BeginFrameTimers();
void RogueDebug_EndFrameTimers();
void RogueDebug_PrintTimers();


#ifdef DEBUG_FEATURE_MEMORY_STOMP_TRACKING

#define MEMORY_STOMP_TRACKING_SET_TARGET(ptr, size) RogueMemStomp_SetTarget(ptr, size)
#define MEMORY_STOMP_TRACKING_POLL() RogueMemStomp_Poll()

void RogueMemStomp_SetTarget(void const* ptr, size_t size);
void RogueMemStomp_Poll();
#else // !DEBUG_FEATURE_MEMORY_STOMP_TRACKING
#define MEMORY_STOMP_TRACKING_SET_TARGET(ptr, size)
#define MEMORY_STOMP_TRACKING_POLL()

#endif // DEBUG_FEATURE_MEMORY_STOMP_TRACKING


#ifdef DEBUG_FEATURE_FRAME_TIMERS

// All supported timers
#define FOR_EACH_TIMER(X) \
    X(FRAME_TOTAL) \
    X(MAIN_CALLBACK_1) \
    X(MAIN_CALLBACK_2) \
    X(MAIN_ROGUE_CALLBACK) \

#define TIMER_ENUM(timer) FRAME_TIMER_ ## timer,

enum
{
    FOR_EACH_TIMER(TIMER_ENUM)
    FRAME_TIMER_COUNT,
};

#undef TIMER_ENUM

#define START_TIMER(timer) RogueDebug_StartTimer(FRAME_TIMER_ ## timer);
#define STOP_TIMER(timer) RogueDebug_StopTimer(FRAME_TIMER_ ## timer);

void RogueDebug_StartTimer(u16 timer);
void RogueDebug_StopTimer(u16 timer);

#else // ! DEBUG_FEATURE_FRAME_TIMERS

#define START_TIMER(timer)
#define STOP_TIMER(timer)

#endif // DEBUG_FEATURE_FRAME_TIMERS

#endif //ROGUE_DEBUG_H
