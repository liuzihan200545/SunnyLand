#include "engine/core/game_app.h"

#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::debug); // 设置日志级别为 trace，输出所有日志
    engine::core::GameApp app;
    app.run();
    return 0;

}