//
// Created by Lenovo on 2026/2/12.
//

#ifndef SUNNYLAND_TIME_H
#define SUNNYLAND_TIME_H
#include <SDL3/SDL_stdinc.h>  // 用于 Uint64

namespace engine::core {

class Time final {
private:
    Uint64 last_time_ = 0;             ///< @brief 上一帧的时间戳 (用于计算 delta)
    Uint64 frame_start_time_ = 0;      ///< @brief 当前帧开始的时间戳 (用于帧率限制)
    double delta_time_ = 0.0;          ///< @brief 未缩放的帧间时间差 (秒)
    double time_scale_ = 1.0;          ///< @brief 时间缩放因子

    // 帧率限制相关
    int target_fps_ = 0;               ///< @brief 目标帧率 (0 表示不限制)
    double target_frame_time_ = 0.0;   ///< @brief 目标帧时间 (秒)

public:
    Time();

    // 删除拷贝和移动构造函数/赋值运算符
    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;
    Time(Time&&) = delete;
    Time& operator=(Time&&) = delete;

    /**
     * @brief 每帧开始时调用，更新 delta_time 和帧率限制
     */
    void update();

    /**
     * @brief 获取当前帧的 delta_time（秒），已经应用时间缩放
     */
    [[nodiscard]] float getDeltaTime() const;

    /**
     * @brief 获取当前帧的未缩放 delta_time（秒）
     */
    [[nodiscard]] float getUnscaledDeltaTime() const;


    /**
     * @brief 设置时间缩放因子，默认值为 1.0（正常速度）
     */
    void setTimeScale(double scale);

    /**
     * @brief 获取当前的时间缩放因子
     */
    [[nodiscard]] double getTimeScale() const;

    /**
     * @brief 设置目标帧率，0 表示不限制
     */
    void setTargetFPS(int fps);

    /**
     * @brief 获取当前的目标帧率
     */
    [[nodiscard]] int getTargetFPS() const;

private:
    /**
     * @brief update 中调用，用于限制帧率。如果设置了 target_fps_ > 0，且当前帧执行时间小于目标帧时间，则会调用 SDL_DelayNS() 来等待剩余时间。
     *
     * @param current_delta_time 当前帧的执行时间（秒）
     */
    void limitFrameRate(float current_delta_time);
};

}


#endif //SUNNYLAND_TIME_H