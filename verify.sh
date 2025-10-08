#!/bin/bash

# Verification script for Real-Time Edge Detection Viewer
# Run this to verify your environment is properly configured

echo "🔍 Real-Time Edge Detection Viewer - Environment Verification"
echo "=============================================================="
echo ""

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check functions
check_pass() {
    echo -e "${GREEN}✓${NC} $1"
}

check_fail() {
    echo -e "${RED}✗${NC} $1"
}

check_warn() {
    echo -e "${YELLOW}⚠${NC} $1"
}

# 1. Check OpenCV SDK
echo "1. Checking OpenCV SDK..."
if [ -z "$OPENCV_SDK_PATH" ]; then
    check_fail "OPENCV_SDK_PATH environment variable not set"
    echo "   Run: export OPENCV_SDK_PATH=/path/to/opencv-android-sdk"
else
    if [ -d "$OPENCV_SDK_PATH/sdk/native/jni/include" ]; then
        check_pass "OpenCV SDK found at: $OPENCV_SDK_PATH"
    else
        check_fail "OpenCV SDK path exists but structure is incorrect"
    fi
fi
echo ""

# 2. Check Android Studio / SDK
echo "2. Checking Android SDK..."
if [ -z "$ANDROID_HOME" ]; then
    check_warn "ANDROID_HOME not set (optional if using Android Studio)"
else
    check_pass "Android SDK found at: $ANDROID_HOME"
fi
echo ""

# 3. Check NDK
echo "3. Checking NDK..."
if [ -n "$ANDROID_HOME" ] && [ -d "$ANDROID_HOME/ndk" ]; then
    NDK_VERSION=$(ls "$ANDROID_HOME/ndk" | head -1)
    check_pass "NDK found: $NDK_VERSION"
else
    check_warn "NDK not found in ANDROID_HOME (install via Android Studio)"
fi
echo ""

# 4. Check CMake
echo "4. Checking CMake..."
if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -1)
    check_pass "$CMAKE_VERSION"
else
    check_warn "CMake not found in PATH (Android Studio has bundled version)"
fi
echo ""

# 5. Check Node.js (for web viewer)
echo "5. Checking Node.js..."
if command -v node &> /dev/null; then
    NODE_VERSION=$(node --version)
    check_pass "Node.js $NODE_VERSION"
else
    check_fail "Node.js not found (required for web viewer)"
    echo "   Install from: https://nodejs.org/"
fi
echo ""

# 6. Check npm
echo "6. Checking npm..."
if command -v npm &> /dev/null; then
    NPM_VERSION=$(npm --version)
    check_pass "npm $NPM_VERSION"
else
    check_fail "npm not found"
fi
echo ""

# 7. Check Git
echo "7. Checking Git..."
if command -v git &> /dev/null; then
    GIT_VERSION=$(git --version)
    check_pass "$GIT_VERSION"
else
    check_fail "Git not found"
fi
echo ""

# 8. Check project structure
echo "8. Checking project structure..."
REQUIRED_DIRS=(
    "app/src/main/cpp/jni"
    "app/src/main/cpp/gl"
    "app/src/main/java/com/edgedetector"
    "web/src"
)

for dir in "${REQUIRED_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        check_pass "$dir"
    else
        check_fail "$dir not found"
    fi
done
echo ""

# 9. Check key files
echo "9. Checking key files..."
REQUIRED_FILES=(
    "app/build.gradle.kts"
    "app/src/main/cpp/CMakeLists.txt"
    "app/src/main/AndroidManifest.xml"
    "web/package.json"
    "web/tsconfig.json"
    "README.md"
)

for file in "${REQUIRED_FILES[@]}"; do
    if [ -f "$file" ]; then
        check_pass "$file"
    else
        check_fail "$file not found"
    fi
done
echo ""

# 10. Check Git repository
echo "10. Checking Git repository..."
if [ -d ".git" ]; then
    check_pass "Git repository initialized"
    COMMIT_COUNT=$(git rev-list --count HEAD)
    check_pass "$COMMIT_COUNT commits in history"
    
    REMOTE_URL=$(git remote get-url origin 2>/dev/null)
    if [ -n "$REMOTE_URL" ]; then
        check_pass "Remote: $REMOTE_URL"
    else
        check_warn "No remote repository configured"
    fi
else
    check_fail "Not a Git repository"
fi
echo ""

# Summary
echo "=============================================================="
echo "Verification complete!"
echo ""
echo "Next steps:"
echo "1. If OpenCV SDK is missing, download from: https://opencv.org/releases/"
echo "2. Open project in Android Studio"
echo "3. Sync Gradle files"
echo "4. Build and run on device"
echo "5. For web viewer: cd web && npm install && npm run build"
echo ""
echo "For detailed instructions, see:"
echo "  - README.md (comprehensive guide)"
echo "  - SETUP.md (detailed setup)"
echo "  - QUICKSTART.md (quick reference)"
echo "=============================================================="
