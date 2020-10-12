
#include "../utils/logging.h"
#include <oboe/Oboe.h>

#include "AAssetDataSource.h"
#include "FFMpegExtractor.h"

constexpr int kMaxCompressionRatio { 12 };

AAssetDataSource* AAssetDataSource::newFromCompressedAsset(
        AAssetManager &assetManager,
        const char *filename,
        const AudioProperties targetProperties) {

    AAsset *asset = AAssetManager_open(&assetManager, filename, AASSET_MODE_UNKNOWN);
    if (!asset) {
        LOGE("Failed to open asset %s", filename);
        return nullptr;
    }

    off_t assetSize = AAsset_getLength(asset);
    LOGD("Opened %s, size %ld", filename, assetSize);

    // Allocate memory to store the decompressed audio. We don't know the exact
    // size of the decoded data until after decoding so we make an assumption about the
    // maximum compression ratio and the decoded sample format (float for FFmpeg, int16 for NDK).
    const long maximumDataSizeInBytes = kMaxCompressionRatio * assetSize * sizeof(float);
    auto decodedData = new uint8_t[maximumDataSizeInBytes];

    int64_t bytesDecoded = FFMpegExtractor::decode(asset, decodedData, targetProperties);
    auto numSamples = bytesDecoded / sizeof(float);

    // Now we know the exact number of samples we can create a float array to hold the audio data
    auto outputBuffer = std::make_unique<float[]>(numSamples);

    memcpy(outputBuffer.get(), decodedData, (size_t)bytesDecoded);

    delete[] decodedData;
    AAsset_close(asset);

    return new AAssetDataSource(std::move(outputBuffer),
            numSamples,
            targetProperties);
}