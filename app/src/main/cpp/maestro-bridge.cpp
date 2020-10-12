#include <jni.h>
#include <string>
#include "Maestro.h"
#include "Player.h"
#include <android/asset_manager_jni.h>
#include "utils/logging.h"

std::unique_ptr<Maestro> maestro;

extern "C" {

JNIEXPORT jint JNICALL
Java_com_admqueiroga_maestro_MainActivity_startMaestro(
        JNIEnv *env,
        jobject /* this */,
        jobject asset_manager,
        jint sample_rate,
        jint buffer_size) {
    AAssetManager *assetManager = AAssetManager_fromJava(env, asset_manager);
    if (assetManager == nullptr) {
        LOGE("Could not obtain the AAssetManager");
        return 1;
    }

    maestro = std::make_unique<Maestro>(*assetManager, sample_rate, buffer_size);
    LOGI("Maestro started and warmed up");
    return 0;
}

const char *getPlayerTrack(JNIEnv *env, jobject player) {
    jclass playerClass = env->GetObjectClass(player);
    jfieldID trackFieldId = env->GetFieldID(playerClass, "track", "Ljava/lang/String;");
    auto trackField = (jstring) env->GetObjectField(player, trackFieldId);
    jboolean isCopy;
    const char *track = env->GetStringUTFChars(trackField, &isCopy);
    return track;
}

JNIEXPORT jstring JNICALL
Java_com_admqueiroga_maestro_MainActivity_addPlayer(JNIEnv *env, jobject thiz, jobject player) {
    if (maestro == nullptr) {
        LOGE("Maestro not started");
    }
    const char *track = getPlayerTrack(env, player);
    LOGE("Track %s", track);
    maestro->addPlayer(track);
    return env->NewStringUTF(track);
}

JNIEXPORT void JNICALL
Java_com_admqueiroga_maestro_MainActivity_play(JNIEnv *env, jobject thiz) {
    maestro->play();
}

JNIEXPORT void JNICALL
Java_com_admqueiroga_maestro_MainActivity_pause(JNIEnv *env, jobject thiz) {
    maestro->pause();
}

JNIEXPORT void JNICALL
Java_com_admqueiroga_maestro_MainActivity_stop(JNIEnv *env, jobject thiz) {
    maestro->stop();
}

JNIEXPORT void JNICALL
Java_com_admqueiroga_maestro_MainActivity_seek(JNIEnv *env, jobject thiz, jint progress) {
    maestro->seek(progress);
}

JNIEXPORT jboolean JNICALL
Java_com_admqueiroga_maestro_MainActivity_isPlaying(JNIEnv *env, jobject thiz) {
    return ((Player *) maestro.get())->isPlaying();
}

JNIEXPORT jint JNICALL
Java_com_admqueiroga_maestro_MainActivity_currentProgress(JNIEnv *env, jobject thiz) {
    return 100 * maestro->currentFrame() / maestro->totalFrames();
}


JNIEXPORT jboolean JNICALL
Java_com_admqueiroga_maestro_MainActivity_isReady(JNIEnv *env, jobject thiz) {
    return maestro->isReady();
}

JNIEXPORT void JNICALL
Java_com_admqueiroga_maestro_MainActivity_prepare(JNIEnv *env, jobject thiz) {
    maestro->prepare();
}

JNIEXPORT void JNICALL
Java_com_admqueiroga_maestro_MainActivity_playOnly(JNIEnv *env, jobject thiz, jobject player) {
    maestro->playOnly(getPlayerTrack(env, player));
}

JNIEXPORT void JNICALL
Java_com_admqueiroga_maestro_MainActivity_pauseOnly(JNIEnv *env, jobject thiz, jobject player) {
    maestro->pauseOnly(getPlayerTrack(env, player));
}

JNIEXPORT jboolean JNICALL
Java_com_admqueiroga_maestro_MainActivity_togglePlayback(JNIEnv *env, jobject thiz, jobject player) {
    auto playerTrack = getPlayerTrack(env, player);
    auto isTrackPlaying = maestro->isPlaying(playerTrack);
    isTrackPlaying ? maestro->pauseOnly(playerTrack) : maestro->playOnly(playerTrack);
    return !isTrackPlaying;
}

JNIEXPORT jboolean JNICALL
Java_com_admqueiroga_maestro_MainActivity_isTrackPlaying(JNIEnv *env, jobject thiz, jobject player) {
    return maestro->isPlaying(getPlayerTrack(env, player));
}

}