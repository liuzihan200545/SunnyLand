//
// Created by Lenovo on 2026/2/12.
//

#include "audio_manager.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace engine::resource {
AudioManager::AudioManager() {
    // 1. 初始化 SDL 音频子系统 (SDL3_mixer 依赖它)
    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (!SDL_Init(SDL_INIT_AUDIO)) {
            throw std::runtime_error("AudioManager 错误: SDL_Init(AUDIO) 失败: " + std::string(SDL_GetError()));
        }
    }
    // 2. 初始化 SDL_mixer
    MIX_Init();
    MIX_Mixer* raw_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!raw_mixer) {
        throw std::runtime_error("AudioManager 错误: MIX_CreateMixerDevice 失败: " + std::string(SDL_GetError()));
    }
    mixer_.reset(raw_mixer);
    spdlog::debug("当前使用的音频驱动: {}", SDL_GetCurrentAudioDriver());

    // 可以加入 track

    spdlog::trace("AudioManager 构造成功。");
}


AudioManager::~AudioManager() {
    // 停止所有播放
    if (mixer_) {
        MIX_PauseAllTracks(mixer_.get());
    }

    if (!sounds_.empty()) {
        spdlog::debug("AudioManager sounds_ 不为空，调用 clearSounds 处理清理逻辑。");
        clearSounds();       // 调用 clearSounds 处理清理逻辑
    }
    if (!music_.empty()) {
        spdlog::debug("AudioManager music_ 不为空，调用 clearMusic 处理清理逻辑。");
        clearMusic();        // 调用 clearMusic 处理清理逻辑
    }
    //MIX_Quit();
    spdlog::trace("AudioManager 析构成功。");
}

MIX_Audio* AudioManager::loadSound(const std::string& file_path) {
    // 首先检查缓存
    auto it = sounds_.find(file_path);
    if (it != sounds_.end()) {
        return it->second.get();
    }

    // 缓存中不存在，则加载音效
    spdlog::debug("正在加载音效：{}", file_path);
    // SDL3_mixer 加载函数统一为 MIX_LoadAudio
    // 参数3: predecode (true=预解码为PCM，加载慢但播放快；false=流式解码，省内存)
    // 对于音效建议 true，长音乐建议 false。这里为了通用简单设为 true，
    // 如果你有长音乐文件，建议根据文件扩展名或单独的接口来区分。
    MIX_Audio* raw_sound = MIX_LoadAudio(mixer_.get(), file_path.c_str(), true);
    if (!raw_sound) {
        spdlog::error("加载音效 '{}' 失败：{}", file_path, SDL_GetError());
        return nullptr;
    }

    // 使用 unique_ptr 存储到缓存中
    sounds_.emplace(file_path, std::unique_ptr<MIX_Audio, SDLAudioDeleter>(raw_sound));
    spdlog::debug("成功加载并缓存音效：{}", file_path);
    return raw_sound;
}

MIX_Audio* AudioManager::getSound(const std::string& file_path) {
    auto it = sounds_.find(file_path);
    if (it != sounds_.end()) {
        return it->second.get();
    }

    spdlog::warn("音效 '{}' 不在缓存中，尝试加载。", file_path);
    return loadSound(file_path);
}

void AudioManager::unloadSound(const std::string& file_path) {
    auto it = sounds_.find(file_path);
    if (it != sounds_.end()) {
        spdlog::debug("卸载音效：{}", file_path);
        sounds_.erase(it);       // unique_ptr 会处理 MIX_DestroyAudio
    }
    else {
        spdlog::warn("尝试卸载不存在的音效: {}", file_path);
    }
}

void AudioManager::clearSounds() {
    if (!sounds_.empty()) {
        spdlog::debug("正在清除所有 {} 个缓存的音效。", sounds_.size());
        sounds_.clear(); // unique_ptr处理删除
    }
}

MIX_Audio* AudioManager::loadMusic(const std::string& file_path) {
    // 首先检查缓存
    auto it = music_.find(file_path);
    if (it != music_.end()) {
        return it->second.get();
    }

    // 缓存中不存在，则加载音乐
    spdlog::debug("正在加载音乐：{}", file_path);
    // SDL3_mixer 加载函数统一为 MIX_LoadAudio
    // 参数3: predecode (true=预解码为PCM，加载慢但播放快；false=流式解码，省内存)
    // 对于音乐建议 false，音效建议 true。这里为了通用简单设为 false，
    // 如果你有短音乐文件，建议根据文件扩展名或单独的接口来区分。
    MIX_Audio* raw_music = MIX_LoadAudio(mixer_.get(), file_path.c_str(), false);
    if (!raw_music) {
        spdlog::error("加载音乐 '{}' 失败：{}", file_path, SDL_GetError());
        return nullptr;
    }

    // 使用 unique_ptr 存储到缓存中
    music_.emplace(file_path, std::unique_ptr<MIX_Audio, SDLAudioDeleter>(raw_music));
    spdlog::debug("成功加载并缓存音乐：{}", file_path);
    return raw_music;
}

MIX_Audio* AudioManager::getMusic(const std::string& file_path) {
    auto it = music_.find(file_path);
    if (it != music_.end()) {
        return it->second.get();
    }

    spdlog::warn("音乐 '{}' 不在缓存中，尝试加载。", file_path);
    return loadMusic(file_path);
}

void AudioManager::unloadMusic(const std::string& file_path) {
    auto it = music_.find(file_path);
    if (it != music_.end()) {
        spdlog::debug("卸载音乐：{}", file_path);
        music_.erase(it);       // unique_ptr 会处理 MIX_DestroyAudio
    }
    else {
        spdlog::warn("尝试卸载不存在的音乐: {}", file_path);
    }
}

void AudioManager::clearMusic() {
    if (!music_.empty()) {
        spdlog::debug("正在清除所有 {} 个缓存的音乐。", music_.size());
        music_.clear(); // unique_ptr处理删除
    }
}

void AudioManager::clearAudio() {
    clearSounds();
    clearMusic();
}

}// namespace engine::resource
