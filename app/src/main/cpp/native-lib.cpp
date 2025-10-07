#include <jni.h>
#include <android/log.h>

#define LOG_TAG "EdgeDetector"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_edgedetector_NativeProcessor_getVersion(JNIEnv* env, jobject /* this */) {
    return env->NewStringUTF("EdgeDetector Native v1.0");
}
