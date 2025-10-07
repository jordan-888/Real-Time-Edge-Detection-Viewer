#include <jni.h>
#include <android/log.h>
#include "ImageProcessor.h"
#include "../gl/GLRenderer.h"

#define LOG_TAG "JNIBridge"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static ImageProcessor* g_processor = nullptr;
static GLRenderer* g_renderer = nullptr;

extern "C" {

JNIEXPORT void JNICALL
Java_com_edgedetector_NativeProcessor_nativeInit(JNIEnv* env, jobject /* this */) {
    LOGI("Native initialization");
    if (!g_processor) {
        g_processor = new ImageProcessor();
    }
    if (!g_renderer) {
        g_renderer = new GLRenderer();
    }
}

JNIEXPORT void JNICALL
Java_com_edgedetector_NativeProcessor_nativeDestroy(JNIEnv* env, jobject /* this */) {
    LOGI("Native cleanup");
    if (g_processor) {
        delete g_processor;
        g_processor = nullptr;
    }
    if (g_renderer) {
        delete g_renderer;
        g_renderer = nullptr;
    }
}

JNIEXPORT void JNICALL
Java_com_edgedetector_NativeProcessor_processAndRender(
    JNIEnv* env, jobject /* this */,
    jbyteArray yPlane, jbyteArray uPlane, jbyteArray vPlane,
    jint width, jint height,
    jint yRowStride, jint uvRowStride, jint uvPixelStride,
    jboolean applyFilter) {

    if (!g_processor || !g_renderer) {
        LOGE("Processor or renderer not initialized");
        return;
    }

    // Get byte array pointers
    jbyte* yData = env->GetByteArrayElements(yPlane, nullptr);
    jbyte* uData = env->GetByteArrayElements(uPlane, nullptr);
    jbyte* vData = env->GetByteArrayElements(vPlane, nullptr);

    if (!yData || !uData || !vData) {
        LOGE("Failed to get byte array elements");
        return;
    }

    try {
        // Convert YUV to RGBA
        cv::Mat rgbaMat = ImageProcessor::yuv420ToRgba(
            (uint8_t*)yData, (uint8_t*)uData, (uint8_t*)vData,
            width, height, yRowStride, uvRowStride, uvPixelStride
        );

        // Process frame
        cv::Mat processed = g_processor->processFrame(rgbaMat, applyFilter);

        // Render to OpenGL texture
        if (!processed.empty()) {
            g_renderer->updateTexture(processed.data, processed.cols, processed.rows);
            g_renderer->render();
        }

    } catch (const std::exception& e) {
        LOGE("Exception in processAndRender: %s", e.what());
    }

    // Release byte arrays
    env->ReleaseByteArrayElements(yPlane, yData, JNI_ABORT);
    env->ReleaseByteArrayElements(uPlane, uData, JNI_ABORT);
    env->ReleaseByteArrayElements(vPlane, vData, JNI_ABORT);
}

JNIEXPORT void JNICALL
Java_com_edgedetector_NativeProcessor_onSurfaceCreated(JNIEnv* env, jobject /* this */) {
    LOGI("Surface created");
    if (g_renderer) {
        g_renderer->onSurfaceCreated();
    }
}

JNIEXPORT void JNICALL
Java_com_edgedetector_NativeProcessor_onSurfaceChanged(JNIEnv* env, jobject /* this */, jint width, jint height) {
    LOGI("Surface changed: %dx%d", width, height);
    if (g_renderer) {
        g_renderer->onSurfaceChanged(width, height);
    }
}

} // extern "C"
