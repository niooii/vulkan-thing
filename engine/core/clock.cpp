#include "clock.h"

namespace Engine::Core {

    Clock::Clock()
    : start_time{AbsoluteTimeSeconds()}, since_epoch{0}, time_at_pause{0} {};

    Clock::Clock(bool start_paused)
    : is_paused{start_paused}, start_time{AbsoluteTimeSeconds()}, since_epoch{0}, time_at_pause{AbsoluteTimeSeconds()} {};

    f64 Clock::ElapsedSeconds() {
        if(!is_paused) {
            Update();
        }
        spdlog::warn("START TIME: {}", start_time);
        spdlog::warn("SINCE EPOCH: {}", since_epoch);
        return since_epoch - start_time;
    }

    void Clock::Pause() {
        time_at_pause = AbsoluteTimeSeconds();
        is_paused = true;
    }

    void Clock::Resume() {
        start_time += (AbsoluteTimeSeconds() - time_at_pause);
        spdlog::warn("START TIME: {}", start_time);
        time_at_pause = 0;
        is_paused = false;
    }

    void Clock::Reset() {
        start_time = AbsoluteTimeSeconds();
        since_epoch = AbsoluteTimeSeconds();
    }

    void Clock::Update() {
        since_epoch = AbsoluteTimeSeconds();
    }

    f64 Clock::AbsoluteTimeSeconds() {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }

}