#include "GLRenderer.h"
#include <cstring>

#define LOG_TAG "GLRenderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

const char* GLRenderer::vertexShaderSource = R"(
attribute vec4 a_Position;
attribute vec2 a_TexCoord;
varying vec2 v_TexCoord;

void main() {
    gl_Position = a_Position;
    v_TexCoord = a_TexCoord;
}
)";

const char* GLRenderer::fragmentShaderSource = R"(
precision mediump float;
uniform sampler2D u_Texture;
varying vec2 v_TexCoord;

void main() {
    gl_FragColor = texture2D(u_Texture, v_TexCoord);
}
)";

GLRenderer::GLRenderer() 
    : shaderProgram(0), textureId(0), vbo(0), ebo(0),
      surfaceWidth(0), surfaceHeight(0), textureWidth(0), textureHeight(0) {
    LOGI("GLRenderer created");
}

GLRenderer::~GLRenderer() {
    if (textureId) glDeleteTextures(1, &textureId);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
    if (shaderProgram) glDeleteProgram(shaderProgram);
    LOGI("GLRenderer destroyed");
}

void GLRenderer::onSurfaceCreated() {
    LOGI("onSurfaceCreated");
    
    // Set clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Create shader program
    shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    if (!shaderProgram) {
        LOGE("Failed to create shader program");
        return;
    }
    
    // Get attribute and uniform locations
    positionAttrib = glGetAttribLocation(shaderProgram, "a_Position");
    texCoordAttrib = glGetAttribLocation(shaderProgram, "a_TexCoord");
    textureUniform = glGetUniformLocation(shaderProgram, "u_Texture");
    
    // Generate texture
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Setup geometry
    setupGeometry();
    
    LOGI("OpenGL setup complete");
}

void GLRenderer::onSurfaceChanged(int width, int height) {
    LOGI("onSurfaceChanged: %dx%d", width, height);
    surfaceWidth = width;
    surfaceHeight = height;
    glViewport(0, 0, width, height);
}

void GLRenderer::setupGeometry() {
    // Full-screen quad vertices (position + texcoord)
    float vertices[] = {
        // positions        // texcoords
        -1.0f,  1.0f,       0.0f, 0.0f,  // top-left
         1.0f,  1.0f,       1.0f, 0.0f,  // top-right
         1.0f, -1.0f,       1.0f, 1.0f,  // bottom-right
        -1.0f, -1.0f,       0.0f, 1.0f   // bottom-left
    };
    
    unsigned short indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    
    // Create VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Create EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void GLRenderer::updateTexture(const unsigned char* data, int width, int height) {
    if (!data || width <= 0 || height <= 0) {
        LOGE("Invalid texture data");
        return;
    }
    
    textureWidth = width;
    textureHeight = height;
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void GLRenderer::render() {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!shaderProgram || !textureId) {
        return;
    }
    
    // Use shader program
    glUseProgram(shaderProgram);
    
    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureUniform, 0);
    
    // Bind VBO and set vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    // Bind EBO and draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    
    // Cleanup
    glDisableVertexAttribArray(positionAttrib);
    glDisableVertexAttribArray(texCoordAttrib);
}

GLuint GLRenderer::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    if (!shader) {
        LOGE("Failed to create shader");
        return 0;
    }
    
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            LOGE("Shader compile error: %s", infoLog);
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

GLuint GLRenderer::createProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) return 0;
    
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        glDeleteShader(vertexShader);
        return 0;
    }
    
    GLuint program = glCreateProgram();
    if (!program) {
        LOGE("Failed to create program");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = new char[infoLen];
            glGetProgramInfoLog(program, infoLen, nullptr, infoLog);
            LOGE("Program link error: %s", infoLog);
            delete[] infoLog;
        }
        glDeleteProgram(program);
        program = 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}
