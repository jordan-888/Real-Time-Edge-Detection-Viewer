#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

class GLRenderer {
public:
    GLRenderer();
    ~GLRenderer();

    void onSurfaceCreated();
    void onSurfaceChanged(int width, int height);
    void updateTexture(const unsigned char* data, int width, int height);
    void render();

private:
    GLuint compileShader(GLenum type, const char* source);
    GLuint createProgram(const char* vertexSource, const char* fragmentSource);
    void setupGeometry();

    GLuint shaderProgram;
    GLuint textureId;
    GLuint vbo;
    GLuint ebo;
    
    GLint positionAttrib;
    GLint texCoordAttrib;
    GLint textureUniform;

    int surfaceWidth;
    int surfaceHeight;
    int textureWidth;
    int textureHeight;

    static const char* vertexShaderSource;
    static const char* fragmentShaderSource;
};

#endif // GLRENDERER_H
