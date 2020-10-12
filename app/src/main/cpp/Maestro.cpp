//
// Created by Ademir Queiroga on 05/05/21.
//

#include "Maestro.h"

#include "utils/logging.h"
#include "AudioPlayer.h"

void Maestro::prepare() {
    oboe::AudioStreamBuilder builder;
    builder.setDirection(oboe::Direction::Output)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setFormat(oboe::AudioFormat::Float)
            ->setChannelCount(oboe::ChannelCount::Stereo)
            ->setDataCallback(this);

    if (sampleRate != -1) {
        builder.setSampleRate(sampleRate);
    }
    if (bufferSize != -1) {
        builder.setBufferCapacityInFrames(bufferSize);
    }

    oboe::Result result = builder.openStream(&stream);
    if (result != oboe::Result::OK) {
        LOGE("Error opening stream %s", oboe::convertToText(result));
    }

    stream->setBufferSizeInFrames(stream->getFramesPerBurst() * 2);
    oboe::AudioFormat audioFormat = stream->getFormat();
    LOGI("AudioStream format is %s", oboe::convertToText(audioFormat));

    audioProperties = {
            .channelCount = stream->getChannelCount(),
            .sampleRate = stream->getSampleRate()
    };

    LOGE("Maestro properies ch:%d sr:%d", audioProperties.channelCount, audioProperties.sampleRate);

    for (auto &entry : playersMap) {
        LOGE("Preparing player");
        entry.second->prepare();
    }
}

void Maestro::pause() {
    Player::pause();
}

void Maestro::play() {
    Player::play();
    if (stream->getState() != oboe::StreamState::Started) {
        stream->requestStart();
    }
}

void Maestro::stop() {
    Player::stop();
    for (auto &entry : playersMap) {
        entry.second->stop();
    }
    stream->requestStop();
}


void Maestro::addPlayer(const char *trackPath) {
    LOGE("inserting %s", trackPath);
    auto player = std::make_shared<AudioPlayer>(assetManager,audioProperties,trackPath);
    playersMap[std::string(trackPath)] = player;
    if (player == nullptr) {
        LOGE("player NULL PTR");
    } else {
        LOGE("player PTR");
    }
}

void Maestro::seek(int32_t percentage) {
    auto frameToSeek = (percentage * playersMap.begin()->second->totalFrames()) / 100;
    for (auto &entry : playersMap) {
        entry.second->seek(frameToSeek);
    }
}

void Maestro::render(oboe::AudioStream *oboeStream, float *targetData, int32_t numFrames) {
    int32_t bufferLength = oboeStream->getChannelCount() * numFrames;

    memset(targetData, 0, bufferLength * sizeof(float));
    float copyBuffer[bufferLength];
    if (mIsPlaying) {
        for (auto &entry : playersMap) {
            auto player = entry.second;
            player->render(oboeStream, copyBuffer, numFrames);
            for (int i = 0; i < bufferLength; i++) {
                targetData[i] = targetData[i] + copyBuffer[i];
            }
        }
    } else {
        for (int i = 0; i < bufferLength; i++) {
            targetData[i] = 0;
        }
    }

    readFrameIndex = playersMap.begin()->second->currentFrame();
}

int64_t Maestro::totalFrames() {
    return playersMap.begin()->second->totalFrames();
}

int64_t Maestro::currentFrame() {
    return readFrameIndex;
}

void Maestro::pauseOnly(const char *track) {
    playersMap[std::string(track)]->pause();
}

void Maestro::playOnly(const char *track) {
    playersMap[std::string(track)]->play();
}
