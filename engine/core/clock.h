#ifndef VULKAN_STUFF_CLOCK_H
#define VULKAN_STUFF_CLOCK_H

#include "defines.h"

#include <chrono>

namespace Engine::Core {

    class Clock {
    public:
        // This will start running from the moment it's instantiated.
        Clock();
        // If true, the clock_ will start in a paused state.
        explicit Clock(bool start_paused);

        f64 ElapsedSeconds();
        void Pause();
        void Resume();
        void Reset();

    private:
        f64 start_time;
        f64 since_epoch;
        f64 time_at_pause;
        bool is_paused{false};

        void Update();
        static f64 AbsoluteTimeSeconds();
    };

}

#endif
