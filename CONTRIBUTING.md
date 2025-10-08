# Contributing

Want to help improve this project? Great! Here's how.

## Found a bug?

1. Check if someone already reported it in Issues
2. If not, create a new issue with:
   - What happened
   - What you expected
   - Steps to reproduce
   - Your device/Android version

## Want to add a feature?

1. Fork the repo
2. Create a branch: `git checkout -b my-feature`
3. Make your changes
4. Test on a real device
5. Commit: `git commit -m "feat: describe what you added"`
6. Push and create a pull request

## Commit format

Use these prefixes:
- `feat:` - new feature
- `fix:` - bug fix
- `docs:` - documentation
- `style:` - formatting
- `refactor:` - code cleanup

## Setup for development

1. Get OpenCV SDK from opencv.org
2. Set `export OPENCV_SDK_PATH=/path/to/opencv-sdk`
3. Open in Android Studio
4. Build and test on device

## Testing

Before submitting:
- Build with `./gradlew clean build`
- Test on real device
- Check camera and edge detection work
- For web: `cd web && npm run build`

## What needs work

- More image filters
- Better performance
- Unit tests
- Dark mode UI

Questions? Open an issue.

Thanks for helping out!
