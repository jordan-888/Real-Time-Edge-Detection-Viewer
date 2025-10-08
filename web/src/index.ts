// Sample base64 image data (placeholder - will be replaced with actual processed frame)
const sampleBase64Image: string = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNk+M9QDwADhgGAWjR9awAAAABJRU5ErkJggg==";

interface FrameStats {
    resolution: string;
    fps: string;
    filterType: string;
    processingTime: string;
}

class FrameViewer {
    private frameImage: HTMLImageElement | null;
    private statsDiv: HTMLElement | null;

    constructor() {
        this.frameImage = document.getElementById('processed-frame') as HTMLImageElement;
        this.statsDiv = document.getElementById('frame-stats');
        this.initialize();
    }

    private initialize(): void {
        console.log('Frame Viewer initialized');
        this.displayFrame();
        this.displayStats();
    }

    private displayFrame(): void {
        if (this.frameImage) {
            // In production, this would be replaced with actual frame data from the Android app
            this.frameImage.src = sampleBase64Image;
            this.frameImage.alt = "Processed Edge Detection Frame";
            console.log('Frame displayed');
        }
    }

    private displayStats(): void {
        if (this.statsDiv) {
            const stats: FrameStats = {
                resolution: "1280x720",
                fps: "Static Frame",
                filterType: "Canny Edge Detection",
                processingTime: "~16ms (estimated)"
            };

            this.statsDiv.innerHTML = `
                <div class="stat-item">
                    <strong>Resolution:</strong> ${stats.resolution}
                </div>
                <div class="stat-item">
                    <strong>FPS:</strong> ${stats.fps}
                </div>
                <div class="stat-item">
                    <strong>Filter:</strong> ${stats.filterType}
                </div>
                <div class="stat-item">
                    <strong>Processing Time:</strong> ${stats.processingTime}
                </div>
            `;
            console.log('Stats displayed', stats);
        }
    }

    public updateFrame(base64Data: string): void {
        if (this.frameImage) {
            this.frameImage.src = base64Data;
        }
    }

    public updateStats(stats: FrameStats): void {
        if (this.statsDiv) {
            this.statsDiv.innerHTML = `
                <div class="stat-item">
                    <strong>Resolution:</strong> ${stats.resolution}
                </div>
                <div class="stat-item">
                    <strong>FPS:</strong> ${stats.fps}
                </div>
                <div class="stat-item">
                    <strong>Filter:</strong> ${stats.filterType}
                </div>
                <div class="stat-item">
                    <strong>Processing Time:</strong> ${stats.processingTime}
                </div>
            `;
        }
    }
}

// Initialize viewer when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    const viewer = new FrameViewer();
    console.log('Edge Detection Viewer ready');
});
