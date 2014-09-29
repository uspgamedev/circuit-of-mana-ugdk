#ifndef UGDK_COMMON_CONFIG_H_
#define UGDK_COMMON_CONFIG_H_

// Version
#define UGDK_VERSION_MAJOR 0
#define UGDK_VERSION_MINOR 5
#define UGDK_VERSION_PATCH 0

#define UGDK_BIGVERSION "0.5"
#define UGDK_VERSION "0.5.0"

///
/* #undef CHECKMAC */
/* #undef UGDK_INSTALL_LOCATION */
///

/* #undef UGDK_SWIG_ENABLED */
/* #undef UGDK_LUA_ENABLED */
/* #undef UGDK_PYTHON_ENABLED */
/* #undef UGDK_3D_ENABLED */
#define UGDK_ENABLED(X) UGDK_##X##_ENABLED

#ifdef ANDROID
#include <cstdio>
#include <jni.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "UGDK"
#define printf(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define fprintf(A, ...) printf(__VA_ARGS__)

#endif

#endif // UGDK_COMMON_CONFIG_H_
