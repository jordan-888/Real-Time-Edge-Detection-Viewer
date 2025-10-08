# Real-Time Edge Detection Viewer

An Android app that captures camera frames, processes them with OpenCV in C++ via JNI, and renders the output using OpenGL ES 2.0. Includes a TypeScript web viewer for displaying processed frames.

## Features

**Core Implementation:**
- Camera2 API for live video capture (YUV_420_888 format)
- JNI bridge for Java-to-C++ communication
- OpenCV Canny edge detection in native C++
- OpenGL ES 2.0 hardware-accelerated rendering
- TypeScript web viewer with frame statistics

**Bonus Features:**
- Real-time FPS counter
- Toggle between original and edge-detected view
- Modular architecture (`/app`, `/jni`, `/gl`, `/web`)

## Quick Start

### 1. Prerequisites

Download and install:
- Android Studio (with NDK & CMake)
- OpenCV Android SDK 4.x from [opencv.org/releases](https://opencv.org/releases/)
- Node.js 16+ (for web viewer)

### 2. Setup OpenCV

```bash
# Download and extract OpenCV Android SDK
cd ~
wget https://github.com/opencv/opencv/releases/download/4.8.0/opencv-4.8.0-android-sdk.zip
unzip opencv-4.8.0-android-sdk.zip
mv OpenCV-android-sdk opencv-android-sdk

# Set environment variable (add to ~/.zshrc for persistence)
export OPENCV_SDK_PATH=~/opencv-android-sdk
```

### 3. Build Android App

```bash
# Clone repository
git clone https://github.com/jordan-888/Real-Time-Edge-Detection-Viewer.git
cd Real-Time-Edge-Detection-Viewer

# Open in Android Studio
open -a "Android Studio" .

# In Android Studio:
# 1. Wait for Gradle sync
# 2. Build → Make Project
# 3. Run on physical device (camera required)
```

### 4. Build Web Viewer

```bash
cd web
npm install
npm run build
open index.html
```

## Architecture

**Data Pipeline:**
```
Camera2 → YUV Image → JNI → C++ (OpenCV) → OpenGL Texture → Display
```

**Project Structure:**
```
app/src/main/
├── java/com/edgedetector/
│   ├── MainActivity.kt          # Camera setup, UI, FPS counter
│   ├── CameraManager.kt         # Camera2 API wrapper
│   ├── EdgeGLRenderer.kt        # OpenGL renderer
│   └── NativeProcessor.kt       # JNI interface
├── cpp/
│   ├── jni/
│   │   ├── ImageProcessor.cpp   # OpenCV Canny edge detection
│   │   └── JNIBridge.cpp        # JNI implementation
│   ├── gl/
│   │   └── GLRenderer.cpp       # OpenGL ES 2.0 rendering
│   └── CMakeLists.txt           # Native build config
└── res/layout/
    └── activity_main.xml        # UI layout

web/
├── src/index.ts                 # TypeScript viewer
├── index.html                   # Web UI
└── package.json
```

## Technical Details

**OpenCV Processing:**
```cpp
YUV → RGBA → Grayscale → Blur (3x3) → Canny (50, 150) → RGBA
```

**Performance:**
- Target: 10-15 FPS
- Resolution: 1280x720
- Processing: ~16ms/frame
- Memory: ~80MB

**JNI Optimization:**
- Direct buffer access
- Minimal memory copying
- Native processing pipeline

**OpenGL Rendering:**
- Vertex/Fragment shaders
- Texture mapping
- Full-screen quad

## Usage

1. Launch app and grant camera permission
2. Edge detection applies automatically
3. Tap "Toggle Filter" to switch between original/processed
4. FPS counter shows performance

## Troubleshooting

**OpenCV SDK not found:**
```bash
# Verify path
ls $OPENCV_SDK_PATH/sdk/native/jni/include

# If missing, re-download and set OPENCV_SDK_PATH
export OPENCV_SDK_PATH=/path/to/opencv-android-sdk
```

**NDK not configured:**
- Android Studio → Tools → SDK Manager → SDK Tools
- Install: NDK (Side by side), CMake

**Build fails:**
```bash
./gradlew clean
./gradlew build --refresh-dependencies
```

**Low FPS:**
- Reduce resolution in `CameraManager.chooseOptimalSize()`
- Test on newer device

## Development

**Git History:**
```bash
git log --oneline
# Shows atomic commits following Conventional Commits standard
```

**Commit Examples:**
- `feat: initial project setup`
- `build: integrate OpenCV via CMake`
- `feat(camera): implement Camera2 API`
- `feat(jni): establish data bridge`
- `feat(opencv): implement Canny edge detection`
- `feat(opengl): render processed frames`
- `feat(app): add FPS counter and filter toggle`
- `feat(web): create TypeScript viewer`

**Testing:**
```bash
# Install and monitor logs
adb install app/build/outputs/apk/debug/app-debug.apk
adb logcat -s EdgeDetector:V CameraManager:V GLRenderer:V
```

## Requirements

- Android SDK 24+ (Android 7.0+)
- Physical device with camera
- OpenGL ES 2.0 support

## Resources

- [OpenCV Docs](https://docs.opencv.org/)
- [Camera2 API Guide](https://developer.android.com/training/camera2)
- [OpenGL ES Tutorial](https://developer.android.com/guide/topics/graphics/opengl)
- [JNI Best Practices](https://developer.android.com/training/articles/perf-jni)
