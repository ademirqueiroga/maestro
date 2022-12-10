//
// Created by Ademir Queiroga on 05/05/21.
//

#ifndef MAESTRO_LOGGING_H
#define MAESTRO_LOGGING_H

#include <stdio.h>
#include <android/log.h>
#include <vector>

#define APP_NAME "Maestro"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, APP_NAME, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__))

#endif //MAESTRO_LOGGING_H
