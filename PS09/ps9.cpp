#include "fssimplewindow.h"
#include "yspng.h"
#include <iostream>

void BinarizeImage(YsRawPngDecoder& png) {
    for(int y = 0; y < png.hei; ++y) {
        for(int x = 0; x < png.wid; ++x) {
            int idx = (y * png.wid + x) * 4;
            bool makeBlack = false;
            
            // Check if any component is less than 220
            for(int i = 0; i < 3; ++i) {
                if(png.rgba[idx + i] < 220) {
                    makeBlack = true;
                    break;
                }
            }
            
            // Set all components to either 0 or 255
            unsigned char val = makeBlack ? 0 : 255;
            png.rgba[idx] = val;
            png.rgba[idx + 1] = val;
            png.rgba[idx + 2] = val;
        }
    }
}

void ExtractContour(YsRawPngDecoder& png) {
    // First pass: mark non-contour white pixels by clearing green channel
    for(int y = 1; y < png.hei - 1; ++y) {
        for(int x = 1; x < png.wid - 1; ++x) {
            int idx = (y * png.wid + x) * 4;
            
            // Only process white pixels
            if(png.rgba[idx] == 255) {
                bool allWhite = true;
                
                // Check all 8 neighbors
                for(int dy = -1; dy <= 1 && allWhite; ++dy) {
                    for(int dx = -1; dx <= 1; ++dx) {
                        if(dx == 0 && dy == 0) continue;
                        
                        int nIdx = ((y + dy) * png.wid + (x + dx)) * 4;
                        if(png.rgba[nIdx] == 0) {
                            allWhite = false;
                            break;
                        }
                    }
                }
                
                // If all neighbors are white, mark this pixel
                if(allWhite) {
                    png.rgba[idx + 1] = 0;  // Clear green
                }
            }
        }
    }
    
    // Second pass: apply changes based on green channel
    for(int y = 0; y < png.hei; ++y) {
        for(int x = 0; x < png.wid; ++x) {
            int idx = (y * png.wid + x) * 4;
            if(png.rgba[idx + 1] == 0) {  // If green was cleared
                png.rgba[idx] = 0;      // Make pixel black
                png.rgba[idx + 2] = 0;
            }
        }
    }
}

int main(void) {
    FsChangeToProgramDir();  // Change to program directory first as required
    
    YsRawPngDecoder png;
    if(YSOK != png.Decode("ps9.png")) {
        std::cout << "Failed to load image.\n";
        return 1;
    }
    
    FsOpenWindow(0, 0, png.wid, png.hei, 1);
    
    while(true) {
        FsPollDevice();
        auto key = FsInkey();
        
        if(FSKEY_ESC == key) {
            break;
        }
        if(FSKEY_SPACE == key) {
            BinarizeImage(png);
        }
        if(FSKEY_ENTER == key) {
            ExtractContour(png);
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glRasterPos2i(0, png.hei - 1);
        glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
        FsSwapBuffers();
        
        FsSleep(16);
    }
    
    return 0;
}
