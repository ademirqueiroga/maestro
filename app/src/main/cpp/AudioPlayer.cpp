//
// Created by Ademir Queiroga on 05/05/21.
//

#include "AudioPlayer.h"
#include "audio/AAssetDataSource.h"
#include <utility>
#include "utils/logging.h"

void AudioPlayer::prepare() {
    LOGE("AudioPlayer prepare ch:%d sr:%d", audioProperties.channelCount,
         audioProperties.sampleRate);
    state = Player::State::Preparing;
    sourceFuture = std::async([&]() {
        std::shared_ptr<AAssetDataSource> audioSource{
                AAssetDataSource::newFromCompressedAsset(assetManager, trackPath, audioProperties)
        };
        source = audioSource;
        state = Player::State::Ready;
    LOGE("Player ready %s", trackPath);
    });
}

void AudioPlayer::render(oboe::AudioStream *oboeStream, float *targetData, int32_t numFrames) {
    int64_t framesToRenderFromData = numFrames;
    int64_t totalSourceFrames = source->getSize() / audioProperties.channelCount;

    if (readFrameIndex + numFrames >= totalSourceFrames) {
        framesToRenderFromData = totalSourceFrames - readFrameIndex;
    }

    const float *data = source->getData();
    int32_t channelCount = audioProperties.channelCount;
    for (int frame = 0; frame < numFrames; frame++) {
        for (int channel = 0; channel < audioProperties.channelCount; channel++) {
            if (mIsPlaying) {
                targetData[(frame * channelCount) + channel] =
                        data[(readFrameIndex * channelCount) + channel];
            } else {
                targetData[(frame * channelCount) + channel] = 0;
            }
        }
        if (++readFrameIndex >= totalSourceFrames) {
            readFrameIndex = 0;
        }
    }
}

int64_t AudioPlayer::totalFrames() {
    return source->getSize() / source->getProperties().channelCount;
}


