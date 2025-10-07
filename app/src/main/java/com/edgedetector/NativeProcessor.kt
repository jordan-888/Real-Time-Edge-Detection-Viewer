package com.edgedetector

object NativeProcessor {
    init {
        System.loadLibrary("edgedetector")
    }

    external fun nativeInit()
    external fun nativeDestroy()
    
    external fun processAndRender(
        yPlane: ByteArray,
        uPlane: ByteArray,
        vPlane: ByteArray,
        width: Int,
        height: Int,
        yRowStride: Int,
        uvRowStride: Int,
        uvPixelStride: Int,
        applyFilter: Boolean
    )
    
    external fun onSurfaceCreated()
    external fun onSurfaceChanged(width: Int, height: Int)
}
