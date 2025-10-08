package com.edgedetector

import android.Manifest
import android.content.pm.PackageManager
import android.media.Image
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.nio.ByteBuffer

class MainActivity : AppCompatActivity() {
    
    private val TAG = "MainActivity"
    private val CAMERA_PERMISSION_REQUEST = 100
    
    private lateinit var glSurfaceView: GLSurfaceView
    private lateinit var fpsText: TextView
    private lateinit var toggleButton: Button
    
    private lateinit var cameraManager: CameraManager
    private lateinit var glRenderer: EdgeGLRenderer
    
    private var filterEnabled = true
    private var frameCount = 0
    private var lastFpsTime = System.currentTimeMillis()
    private var currentFps = 0.0f
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        glSurfaceView = findViewById(R.id.glSurfaceView)
        fpsText = findViewById(R.id.fpsText)
        toggleButton = findViewById(R.id.toggleButton)
        
        // Initialize native processor
        try {
            val version = NativeProcessor.getVersion()
            Log.i(TAG, "Native library loaded: $version")
            NativeProcessor.nativeInit()
        } catch (e: Exception) {
            Log.e(TAG, "Failed to load native library", e)
        }
        
        // Setup OpenGL surface
        glSurfaceView.setEGLContextClientVersion(2)
        glRenderer = EdgeGLRenderer()
        glSurfaceView.setRenderer(glRenderer)
        glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        
        // Setup camera manager
        cameraManager = CameraManager(this)
        cameraManager.frameCallback = { image ->
            processFrame(image)
        }
        
        // Toggle button
        toggleButton.setOnClickListener {
            filterEnabled = !filterEnabled
            toggleButton.text = if (filterEnabled) "Disable Filter" else "Enable Filter"
            Log.i(TAG, "Filter ${if (filterEnabled) "enabled" else "disabled"}")
        }
        
        // Check camera permission
        if (checkCameraPermission()) {
            startCamera()
        } else {
            requestCameraPermission()
        }
    }
    
    private fun processFrame(image: Image) {
        try {
            val planes = image.planes
            if (planes.size < 3) {
                Log.e(TAG, "Invalid image planes")
                return
            }
            
            // Extract YUV planes
            val yPlane = planes[0]
            val uPlane = planes[1]
            val vPlane = planes[2]
            
            val yBuffer = yPlane.buffer
            val uBuffer = uPlane.buffer
            val vBuffer = vPlane.buffer
            
            val yData = ByteArray(yBuffer.remaining())
            val uData = ByteArray(uBuffer.remaining())
            val vData = ByteArray(vBuffer.remaining())
            
            yBuffer.get(yData)
            uBuffer.get(uData)
            vBuffer.get(vData)
            
            // Process and render in native code
            NativeProcessor.processAndRender(
                yData, uData, vData,
                image.width, image.height,
                yPlane.rowStride, uPlane.rowStride, uPlane.pixelStride,
                filterEnabled
            )
            
            // Request GL render
            glSurfaceView.requestRender()
            
            // Update FPS
            updateFps()
            
        } catch (e: Exception) {
            Log.e(TAG, "Error processing frame", e)
        }
    }
    
    private fun updateFps() {
        frameCount++
        val currentTime = System.currentTimeMillis()
        val elapsed = currentTime - lastFpsTime
        
        if (elapsed >= 1000) {
            currentFps = (frameCount * 1000.0f) / elapsed
            frameCount = 0
            lastFpsTime = currentTime
            
            runOnUiThread {
                fpsText.text = String.format("FPS: %.1f", currentFps)
            }
        }
    }
    
    private fun checkCameraPermission(): Boolean {
        return ContextCompat.checkSelfPermission(
            this,
            Manifest.permission.CAMERA
        ) == PackageManager.PERMISSION_GRANTED
    }
    
    private fun requestCameraPermission() {
        ActivityCompat.requestPermissions(
            this,
            arrayOf(Manifest.permission.CAMERA),
            CAMERA_PERMISSION_REQUEST
        )
    }
    
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        
        if (requestCode == CAMERA_PERMISSION_REQUEST) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                startCamera()
            } else {
                Toast.makeText(this, R.string.permission_required, Toast.LENGTH_LONG).show()
                finish()
            }
        }
    }
    
    private fun startCamera() {
        Log.i(TAG, "Starting camera")
        cameraManager.startCamera()
    }
    
    override fun onResume() {
        super.onResume()
        glSurfaceView.onResume()
    }
    
    override fun onPause() {
        super.onPause()
        glSurfaceView.onPause()
    }
    
    override fun onDestroy() {
        super.onDestroy()
        cameraManager.stopCamera()
        NativeProcessor.nativeDestroy()
    }
}
