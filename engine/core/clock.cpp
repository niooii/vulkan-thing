#include "clock.h"

namespace Engine::Core {

    Clock::Clock()
    : start_time{AbsoluteTimeSeconds()}, elapsed_secs{0}, time_at_pause{0} {};

    Clock::Clock(bool start_paused)
    : is_paused{start_paused}, start_time{AbsoluteTimeSeconds()}, elapsed_secs{0}, time_at_pause{0} {};

    f64 Clock::ElapsedSeconds() {
        if(!is_paused) {
            Update();
        }
        return elapsed_secs;
    }

    void Clock::Pause() {
        time_at_pause = AbsoluteTimeSeconds();
        is_paused = true;
    }

    void Clock::Resume() {
        start_time += (AbsoluteTimeSeconds() - time_at_pause);
        time_at_pause = 0;
        is_paused = false;
    }

    void Clock::Reset() {
        start_time = AbsoluteTimeSeconds();
        elapsed_secs = 0;
    }

    void Clock::Update() {
        elapsed_secs = AbsoluteTimeSeconds();
    }

    f64 Clock::AbsoluteTimeSeconds() {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }

}