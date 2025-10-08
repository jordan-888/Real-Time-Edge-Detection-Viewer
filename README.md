# Real-Time Edge Detection Viewer

Android app that processes camera frames with OpenCV and displays them using OpenGL ES. Built with JNI for performance.

## What it does

- Captures live video from camera
- Applies Canny edge detection in C++
- Renders processed frames with OpenGL
- Shows FPS counter and filter toggle
- Includes web viewer for frame stats

## Setup

**Requirements:**
- Android Studio with NDK
- OpenCV Android SDK 4.x
- Physical Android device

**Quick setup:**
```bash
# Get OpenCV SDK
wget https://github.com/opencv/opencv/releases/download/4.8.0/opencv-4.8.0-android-sdk.zip
unzip opencv-4.8.0-android-sdk.zip
export OPENCV_SDK_PATH=~/OpenCV-android-sdk

# Clone and build
git clone https://github.com/jordan-888/Real-Time-Edge-Detection-Viewer.git
cd Real-Time-Edge-Detection-Viewer
# Open in Android Studio, sync, build, run
```

**Web viewer:**
```bash
cd web && npm install && npm run build && open index.html
```

## How it works

```
Camera → YUV frames → JNI → OpenCV (C++) → OpenGL → Display
```

Key files:
- `MainActivity.kt` - Camera setup and UI
- `CameraManager.kt` - Camera2 API handling
- `ImageProcessor.cpp` - Edge detection logic
- `GLRenderer.cpp` - OpenGL rendering
- `web/` - TypeScript viewer

## Performance

- 10-15 FPS on most devices
- 1280x720 resolution
- ~16ms processing per frame

## Common issues

**OpenCV not found:**
```bash
export OPENCV_SDK_PATH=/path/to/opencv-sdk
```

**Build errors:**
```bash
./gradlew clean build
```

**Low performance:**
- Use newer device
- Lower resolution in `CameraManager.chooseOptimalSize()`

## Development

Uses conventional commits:
```
feat: new feature
fix: bug fix  
docs: documentation
```

Test with:
```bash
adb logcat -s EdgeDetector CameraManager GLRenderer
```

## License

MIT - see LICENSE file
