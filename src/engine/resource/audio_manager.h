//
// Created by Lenovo on 2026/2/12.
//

#ifndef SUNNYLAND_AUDIO_MANAGER_H
#define SUNNYLAND_AUDIO_MANAGER_H

#include <memory>       // 用于 std::unique_ptr
#include <stdexcept>    // 用于 std::runtime_error
#include <string>       // 用于 std::string
#include <unordered_map> // 用于 std::unordered_map

#include <SDL3_mixer/SDL_mixer.h>

namespace engine::resource {
/**
 * @brief 管理 SDL_mixer 音效 (Mix_Chunk) 和音乐 (Mix_Music)。
 *
 * 提供音频资源的加载和缓存功能。构造失败时会抛出异常。
 * 仅供 ResourceManager 内部使用。
 */
class AudioManager final{
    friend class ResourceManager;
private:
    // MIX_Audio 的自定义删除器
    struct SDLAudioDeleter {
        void operator()(MIX_Audio* audio) const {
            if (audio) {
                MIX_DestroyAudio(audio);
            }
        }
    };

    // MIX_Mixer 的自定义删除器
    struct MIX_MixerDeleter {
        void operator()(MIX_Mixer* mixer) const {
            if (mixer) {
                //MIX_DestroyMixer(mixer);
            }
        }
    };

    std::unordered_map<std::string, std::unique_ptr<MIX_Audio,SDLAudioDeleter>> sounds_;
    std::unordered_map<std::string, std::unique_ptr<MIX_Audio,SDLAudioDeleter>> music_;

    std::unique_ptr<MIX_Mixer, MIX_MixerDeleter> mixer_;

public:
    /**
     * @brief 构造函数。初始化 SDL_mixer 并打开音频设备。
     * @throws std::runtime_error 如果 SDL_mixer 初始化或打开音频设备失败。
     */
    AudioManager();

    ~AudioManager();   ///< @brief 需要手动添加析构函数，清理资源并关闭 SDL_mixer。

    AudioManager(const AudioManager&) = delete;            ///< @brief 禁止复制构造函数。
    AudioManager& operator=(const AudioManager&) = delete; ///< @brief 禁止复制赋值运算符。
    AudioManager(AudioManager&&) = delete;                 ///< @brief 禁止移动构造函数。
    AudioManager& operator=(AudioManager&&) = delete;      ///< @brief 禁止移动赋值运算符。

private:

    MIX_Audio* loadSound(const std::string& file_path); ///< @brief 加载音效文件，返回 Mix_Chunk*。
    MIX_Audio* getSound(const std::string& file_path);  ///< @brief 获取已加载的音效，如果未加载则调用 loadSound 加载。

    void unloadSound(const std::string& file_path);
    void clearSounds();

    MIX_Audio* loadMusic(const std::string& file_path); ///< @brief 加载音乐文件，返回 Mix_Music*。
    MIX_Audio* getMusic(const std::string& file_path);  ///< @brief 获取已加载的音乐，如果未加载则调用 loadMusic 加载。

    void unloadMusic(const std::string& file_path);
    void clearMusic();

    void clearAudio();

};



}

#endif //SUNNYLAND_AUDIO_MANAGER_H