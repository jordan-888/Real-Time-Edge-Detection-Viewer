# Contributing to Real-Time Edge Detection Viewer

Thank you for your interest in contributing! This document provides guidelines for contributing to this project.

## How to Contribute

### Reporting Issues

If you find a bug or have a feature request:

1. Check if the issue already exists in the [Issues](https://github.com/jordan-888/Real-Time-Edge-Detection-Viewer/issues) section
2. If not, create a new issue with:
   - Clear, descriptive title
   - Detailed description of the problem/feature
   - Steps to reproduce (for bugs)
   - Expected vs actual behavior
   - Screenshots if applicable
   - Device/OS information

### Submitting Changes

1. **Fork the Repository**
   ```bash
   git clone https://github.com/jordan-888/Real-Time-Edge-Detection-Viewer.git
   cd Real-Time-Edge-Detection-Viewer
   ```

2. **Create a Feature Branch**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b fix/your-bug-fix
   ```

3. **Make Your Changes**
   - Follow the existing code style
   - Write clear, concise commit messages
   - Test your changes thoroughly

4. **Commit Your Changes**
   ```bash
   git add .
   git commit -m "feat: add your feature description"
   ```
   
   Follow [Conventional Commits](https://www.conventionalcommits.org/):
   - `feat:` - New feature
   - `fix:` - Bug fix
   - `docs:` - Documentation changes
   - `style:` - Code style changes (formatting)
   - `refactor:` - Code refactoring
   - `test:` - Adding tests
   - `chore:` - Maintenance tasks

5. **Push to Your Fork**
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create a Pull Request**
   - Go to the original repository
   - Click "New Pull Request"
   - Select your branch
   - Provide a clear description of your changes

## Development Setup

### Prerequisites
- Android Studio with NDK & CMake
- OpenCV Android SDK 4.x
- Node.js 16+ (for web viewer)

### Setup Steps
1. Download OpenCV Android SDK from [opencv.org](https://opencv.org/releases/)
2. Set environment variable:
   ```bash
   export OPENCV_SDK_PATH=/path/to/opencv-android-sdk
   ```
3. Open project in Android Studio
4. Sync Gradle files
5. Build and run

## Code Style Guidelines

### Kotlin
- Follow [Kotlin Coding Conventions](https://kotlinlang.org/docs/coding-conventions.html)
- Use meaningful variable names
- Add comments for complex logic
- Keep functions focused and concise

### C++
- Follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Use RAII for resource management
- Proper memory management (no leaks)
- Add header guards to all `.h` files

### TypeScript
- Follow [TypeScript Style Guide](https://google.github.io/styleguide/tsguide.html)
- Use strict type checking
- Avoid `any` types when possible

## Testing

Before submitting a PR:

1. **Build Test**
   ```bash
   ./gradlew clean build
   ```

2. **Runtime Test**
   - Test on physical device
   - Verify camera functionality
   - Check edge detection works
   - Confirm FPS counter updates
   - Test filter toggle

3. **Web Viewer Test**
   ```bash
   cd web
   npm run build
   # Open index.html and verify
   ```

## Areas for Contribution

### High Priority
- [ ] Add unit tests for image processing
- [ ] Implement additional filters (Sobel, Gaussian, etc.)
- [ ] Performance optimizations
- [ ] Support for more camera formats

### Medium Priority
- [ ] Add filter parameter controls (threshold adjustment)
- [ ] Implement frame export functionality
- [ ] Real-time streaming to web viewer
- [ ] Dark mode UI

### Low Priority
- [ ] Add more documentation
- [ ] Create video tutorials
- [ ] Improve error messages
- [ ] Add localization support

## Questions?

Feel free to open an issue with the `question` label if you need help or clarification.

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (MIT License).

---

Thank you for contributing! 🎉
