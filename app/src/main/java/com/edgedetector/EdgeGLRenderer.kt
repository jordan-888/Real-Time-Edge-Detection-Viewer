package com.edgedetector

import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class EdgeGLRenderer : GLSurfaceView.Renderer {
    
    private val TAG = "EdgeGLRenderer"
    
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.i(TAG, "Surface created")
        NativeProcessor.onSurfaceCreated()
    }
    
    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.i(TAG, "Surface changed: ${width}x${height}")
        NativeProcessor.onSurfaceChanged(width, height)
    }
    
    override fun onDrawFrame(gl: GL10?) {
        // Rendering is handled in native code when frames are processed
    }
}
