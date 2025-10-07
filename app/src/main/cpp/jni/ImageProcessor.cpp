#include "ImageProcessor.h"
#include <android/log.h>

#define LOG_TAG "ImageProcessor"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

ImageProcessor::ImageProcessor() 
    : lowThreshold(50.0), highThreshold(150.0), blurSize(3) {
    LOGI("ImageProcessor initialized");
}

ImageProcessor::~ImageProcessor() {
    LOGI("ImageProcessor destroyed");
}

cv::Mat ImageProcessor::processFrame(const cv::Mat& input, bool applyFilter) {
    if (input.empty()) {
        LOGI("Empty input frame");
        return cv::Mat();
    }

    if (!applyFilter) {
        // Return original frame
        return input.clone();
    }

    cv::Mat gray, blurred, edges, output;

    try {
        // Convert to grayscale
        cv::cvtColor(input, gray, cv::COLOR_RGBA2GRAY);

        // Apply Gaussian blur to reduce noise
        cv::blur(gray, blurred, cv::Size(blurSize, blurSize));

        // Apply Canny edge detection
        cv::Canny(blurred, edges, lowThreshold, highThreshold, 3);

        // Convert back to RGBA for display
        cv::cvtColor(edges, output, cv::COLOR_GRAY2RGBA);

        LOGI("Frame processed: %dx%d", output.cols, output.rows);
        return output;

    } catch (const cv::Exception& e) {
        LOGI("OpenCV exception: %s", e.what());
        return input.clone();
    }
}

cv::Mat ImageProcessor::yuv420ToRgba(const uint8_t* yData, const uint8_t* uData, const uint8_t* vData,
                                      int width, int height, int yRowStride, int uvRowStride, int uvPixelStride) {
    cv::Mat yMat(height, width, CV_8UC1, (void*)yData, yRowStride);
    cv::Mat output(height, width, CV_8UC4);

    // Simple YUV to RGBA conversion
    // For better performance, consider using libyuv or hardware acceleration
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int uvRow = row / 2;
            int uvCol = col / 2;
            
            int yIndex = row * yRowStride + col;
            int uvIndex = uvRow * uvRowStride + uvCol * uvPixelStride;

            int y = yData[yIndex] & 0xFF;
            int u = (uData[uvIndex] & 0xFF) - 128;
            int v = (vData[uvIndex] & 0xFF) - 128;

            // YUV to RGB conversion
            int r = y + (int)(1.370705f * v);
            int g = y - (int)(0.337633f * u) - (int)(0.698001f * v);
            int b = y + (int)(1.732446f * u);

            // Clamp values
            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));

            output.at<cv::Vec4b>(row, col) = cv::Vec4b(r, g, b, 255);
        }
    }

    return output;
}
