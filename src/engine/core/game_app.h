//
// Created by Lenovo on 2026/2/12.
//

#ifndef SUNNYLAND_GAME_APP_H
#define SUNNYLAND_GAME_APP_H

struct SDL_Window;
struct SDL_Renderer;

namespace engine::core {

class GameApp final {
private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* sdl_renderer_ = nullptr;
    bool is_running_ = false;

public:
    GameApp();
    ~GameApp();

    /**
     * @brief 运行游戏应用程序，其中会调用init()，然后进入主循环，离开循环后自动调用close()。
     */
    void run();

    // 禁止拷贝和移动
    GameApp(const GameApp&) = delete;
    GameApp& operator=(const GameApp&) = delete;
    GameApp(GameApp&&) = delete;
    GameApp& operator=(GameApp&&) = delete;

private:
    [[nodiscard]] bool init();
    void handleEvents();
    void update(float dt);
    void render();
    void close();

};


}


#endif //SUNNYLAND_GAME_APP_H