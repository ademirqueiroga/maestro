//
// Created by Ademir Queiroga on 08/05/21.
//

#ifndef MAESTRO_AUDIOPLAYER_H
#define MAESTRO_AUDIOPLAYER_H

#include "Player.h"
#include "audio/AAssetDataSource.h"

class AudioPlayer : public Player {

public:
    explicit AudioPlayer(AAssetManager &assetManager, AudioProperties &audioProperties, const char *trackPath)
            : trackPath(trackPath),
              assetManager(assetManager),
              audioProperties(audioProperties) {};

    void prepare() override;

    void render(oboe::AudioStream *oboeStream, float *targetData, int32_t numFrames) override;

    int64_t totalFrames() override;

private:
    AAssetManager &assetManager;
    AudioProperties &audioProperties;
    const char *trackPath;
    std::shared_ptr<AAssetDataSource> source;
    std::future<void> sourceFuture;
};

#endif //MAESTRO_AUDIOPLAYER_H
