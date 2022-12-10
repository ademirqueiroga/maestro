//
// Created by Ademir Queiroga on 05/05/21.
//

#ifndef MAESTRO_PLAYER_H
#define MAESTRO_PLAYER_H

#include <cstdint>
#include <atomic>
#include <memory>
#include "audio/DataSource.h"
#include "Constants.h"
#include <future>
#include <oboe/oboe.h>
#include <android/asset_manager.h>
#include "utils/logging.h"

class Player {
public:

    enum class State : int32_t {
        Created = 0,
        Preparing = 1,
        Ready = 2,
    };

    virtual void render(oboe::AudioStream *oboeStream, float *targetData, int32_t numFrames) = 0;

    bool isPlaying() { return mIsPlaying; }

    virtual void prepare() = 0;

    virtual void play() { mIsPlaying = true; };

    virtual void pause() { mIsPlaying = false; };

    virtual void stop() {
        mIsPlaying = false;
        readFrameIndex = 0;
    };

    virtual void seek(int32_t frame) { readFrameIndex = frame; };

    virtual int64_t totalFrames() = 0;

    virtual int64_t currentFrame() { return readFrameIndex; };

    State getState() { return state; };

protected:
    int32_t readFrameIndex = 0;
    bool mIsPlaying{true};
    State state{State::Created};
};


#endif //MAESTRO_PLAYER_H
