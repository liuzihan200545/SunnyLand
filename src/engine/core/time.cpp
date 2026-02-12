//
// Created by Lenovo on 2026/2/12.
//

#include "time.h"
#include <spdlog/spdlog.h>
#include <SDL3/SDL_timer.h>    // 用于 SDL_GetTicksNS()

namespace engine::core {
Time::Time() {
    last_time_ = SDL_GetTicksNS();
    frame_start_time_ = last_time_;
    spdlog::trace("Time 初始化。Last time: {}", last_time_);
}

void Time::update() {

    frame_start_time_ = SDL_GetTicksNS(); // 记录进入 update 时的时间戳
    auto current_delta_time = static_cast<float>(frame_start_time_ - last_time_) / 1'000'000'000.0f; // 转换为秒
    if (target_frame_time_ > 0.0f ) {
        limitFrameRate(current_delta_time);
    }
    else {
        delta_time_ = current_delta_time;
    }

    last_time_ = SDL_GetTicksNS(); // 记录离开 update 时的时间戳

}


void Time::limitFrameRate(float current_delta_time) {
    if (current_delta_time < target_frame_time_) {
        double time_to_wait = (target_frame_time_ - current_delta_time);
        auto ns_to_wait = static_cast<Uint64>(time_to_wait * 1000000000.0);
        SDL_DelayNS(ns_to_wait);
    }
    delta_time_ = static_cast<double>(SDL_GetTicksNS() - last_time_) / 1000000000.0;
}

float Time::getDeltaTime() const {
    return static_cast<float>(delta_time_ * time_scale_);
}

float Time::getUnscaledDeltaTime() const {
    return static_cast<float>(delta_time_);
}

void Time::setTimeScale(double scale) {
    time_scale_ = scale;
}

double Time::getTimeScale() const {
    return time_scale_;
}

void Time::setTargetFPS(int fps) {
    if (fps < 0) {
        spdlog::warn("Target FPS 不能为负。Setting to 0 (unlimited).");
        target_fps_ = 0;
    } else {
        target_fps_ = fps;
    }

    if (target_fps_ > 0) {
        target_frame_time_ = 1.0 / static_cast<double>(target_fps_);
        spdlog::info("Target FPS 设置为: {} (Frame time: {:.6f}s)", target_fps_, target_frame_time_);
    } else {
        target_frame_time_ = 0.0;
        spdlog::info("Target FPS 设置为: Unlimited");
    }
}

int Time::getTargetFPS() const {
    return target_fps_;
}


}
