//
// Created by Ademir Queiroga on 29/04/21.
//

#ifndef MULTIPLAYER_FFMPEGEXTRACTOR_H
#define MULTIPLAYER_FFMPEGEXTRACTOR_H

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#include <cstdint>
#include <android/asset_manager.h>
#include "../Constants.h"

class FFMpegExtractor {
public:
    static int64_t decode(AAsset *asset, uint8_t *targetData, AudioProperties targetProperties);

private:
    static bool createAVIOContext(AAsset *asset, uint8_t *buffer, uint32_t bufferSize,
                                  AVIOContext **avioContext);

    static bool createAVFormatContext(AVIOContext *avioContext, AVFormatContext **avFormatContext);

    static bool openAVFormatContext(AVFormatContext *avFormatContext);

    static int32_t cleanup(AVIOContext *avioContext, AVFormatContext *avFormatContext);

    static bool getStreamInfo(AVFormatContext *avFormatContext);

    static AVStream *getBestAudioStream(AVFormatContext *avFormatContext);

    static AVCodec *findCodec(AVCodecID id);

    static void printCodecParameters(AVCodecParameters *params);
};


#endif //MULTIPLAYER_FFMPEGEXTRACTOR_H
