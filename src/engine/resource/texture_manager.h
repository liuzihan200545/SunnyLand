//
// Created by Lenovo on 2026/2/12.
//

#ifndef SUNNYLAND_TEXTURE_MANAGER_H
#define SUNNYLAND_TEXTURE_MANAGER_H

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>
#include <glm/glm.hpp>

namespace engine::resource {

/**
 * @brief 管理 SDL_Texture 资源的加载、存储和检索。
 *
 * 在构造时初始化。使用文件路径作为键，确保纹理只加载一次并正确释放。
 * 依赖于一个有效的 SDL_Renderer，构造失败会抛出异常。
 */
class TextureManager final {
friend class ResourceManager;
private:
    struct SDLTextureDeleter  {
        void operator()(SDL_Texture* texture) const {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
        }
    };

    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> textures_;
    SDL_Renderer* renderer_ = nullptr; // 指向主渲染器的非拥有指针

public:
    /**
    * @brief 构造函数，执行初始化。
    * @param renderer 指向有效的 SDL_Renderer 上下文的指针。不能为空。
    * @throws std::runtime_error 如果 renderer 为 nullptr 或初始化失败。
    */
    explicit TextureManager(SDL_Renderer* renderer);

    // 删除默认构造函数和复制/移动构造函数，禁止复制和移动
    TextureManager(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager& operator=(TextureManager&&) = delete;

private:
    SDL_Texture* loadTexture(const std::string& file_path); ///< @brief 从文件路径加载纹理
    SDL_Texture* getTexture(const std::string& file_path); ///< @brief 尝试获取已加载纹理的指针，如果未加载则尝试加载

    glm::vec2 getTextureSize(const std::string& file_path);
    void unloadTexture(const std::string& file_path);
    void clearTextures();
};


}


#endif //SUNNYLAND_TEXTURE_MANAGER_H