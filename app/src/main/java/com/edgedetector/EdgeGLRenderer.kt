package com.edgedetector

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class EdgeGLRenderer : GLSurfaceView.Renderer {
    
    private val TAG = "EdgeGLRenderer"
    
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.i(TAG, "Surface created - setting red background")
        // Set clear color to red for testing
        GLES20.glClearColor(1.0f, 0.0f, 0.0f, 1.0f)
        Log.i(TAG, "Red color set")
    }
    
    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.i(TAG, "Surface changed: ${width}x${height}")
        GLES20.glViewport(0, 0, width, height)
        Log.i(TAG, "Viewport set")
    }
    
    override fun onDrawFrame(gl: GL10?) {
        // Clear with red color for testing
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)
    }
}
