package com.edgedetector

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class EdgeGLRenderer : GLSurfaceView.Renderer {
    
    private val TAG = "EdgeGLRenderer"
    
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.i(TAG, "Surface created")
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f)
        
        try {
            NativeProcessor.onSurfaceCreated()
        } catch (e: Exception) {
            Log.e(TAG, "Native onSurfaceCreated failed", e)
        }
    }
    
    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.i(TAG, "Surface changed: ${width}x${height}")
        GLES20.glViewport(0, 0, width, height)
        
        try {
            NativeProcessor.onSurfaceChanged(width, height)
        } catch (e: Exception) {
            Log.e(TAG, "Native onSurfaceChanged failed", e)
        }
    }
    
    override fun onDrawFrame(gl: GL10?) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)
    }
}
