//
// Created by Ademir Queiroga on 05/05/21.
//

#ifndef MAESTRO_MAESTRO_H
#define MAESTRO_MAESTRO_H

#include <vector>
#include "AudioPlayer.h"
#include <android/asset_manager.h>
#include <oboe/Oboe.h>
#include <future>
#include <thread>
#include <unordered_map>
#include <string.h>


class Maestro : public oboe::AudioStreamDataCallback, public Player {
public:
    explicit Maestro(AAssetManager &assetManager, int32_t sampleRate, int32_t bufferSize)
            : assetManager(assetManager), sampleRate(sampleRate), bufferSize(bufferSize) {
        mIsPlaying = false;
    };

    ~Maestro() {};

    void prepare() override;

    void addPlayer(const char *trackPath);

    void seek(int32_t frame) override;

    void pause() override;

    void play() override;

    void playOnly(const char *track);

    void pauseOnly(const char *track);

    void stop() override;

    void render(oboe::AudioStream *oboeStream, float *targetData, int32_t numFrames) override;

    int64_t totalFrames() override;

    int64_t currentFrame() override;

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override {
//        if (!mIsPlaying) {
//            return oboe::DataCallbackResult::Stop;
//        }
        render(oboeStream, static_cast<float *>(audioData), numFrames);
        return oboe::DataCallbackResult::Continue;
    }

    bool isReady() {
        return std::all_of(playersMap.begin(), playersMap.end(),
                           [](std::pair<std::string, std::shared_ptr<Player>> pair) {
                               return pair.second->getState() == Player::State::Ready;
                           });
    };

    bool isPlaying(const char *track) {
        return playersMap[std::string(track)]->isPlaying();
    }

private:
    AAssetManager &assetManager;
    oboe::AudioStream *stream;
    AudioProperties audioProperties;
    std::unordered_map<std::string, std::shared_ptr<Player>> playersMap;
    int32_t sampleRate;
    int32_t bufferSize;
};


#endif //MAESTRO_MAESTRO_H
