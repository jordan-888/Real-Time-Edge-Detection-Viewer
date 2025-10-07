#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class ImageProcessor {
public:
    ImageProcessor();
    ~ImageProcessor();

    // Process frame with Canny edge detection
    cv::Mat processFrame(const cv::Mat& input, bool applyFilter);

    // Convert YUV_420_888 to RGBA
    static cv::Mat yuv420ToRgba(const uint8_t* yData, const uint8_t* uData, const uint8_t* vData,
                                 int width, int height, int yRowStride, int uvRowStride, int uvPixelStride);

private:
    double lowThreshold;
    double highThreshold;
    int blurSize;
};

#endif // IMAGEPROCESSOR_H
