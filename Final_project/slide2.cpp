#include "fssimplewindow.h"
#include "yspng.h"
#include "ysglfontdata.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>

struct Label {
    std::string text;
    double startX, startY;    
    double targetX, targetY;  
    double currentX, currentY;
    bool isAnimating;
    double speed;
    bool useSmallFont;
};

void DrawBoldText(const char* str, double x, double y, bool useSmallFont) {
    const double offset = 0.5;
    
    for(double dx = -offset; dx <= offset; dx += offset) {
        for(double dy = -offset; dy <= offset; dy += offset) {
            glRasterPos2d(x + dx, y + dy);
            if(useSmallFont) {
                YsGlDrawFontBitmap8x12(str);
            } else {
                YsGlDrawFontBitmap12x16(str);
            }
        }
    }
}

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 600;
    FsOpenWindow(16, 16, windowWidth, windowHeight, 1);

    // Load first image (wings.png)
    YsRawPngDecoder png;
    if(YSOK != png.Decode("wings.png")) {
        std::cout << "Failed to load wings.png" << std::endl;
        return 1;
    }
    png.Flip();

    // Load second image (wingsssss.png)
    YsRawPngDecoder png2;
    if(YSOK != png2.Decode("wingsssss.png")) {
        std::cout << "Failed to load wingsssss.png" << std::endl;
        return 1;
    }
    png2.Flip();

    std::vector<Label> labels = {
        {"Winglet\n(Decrease Drag)", 
            -200, 100,
            550, 150,
            -200, 100,
            true, 1.0,
            false},
            
        {"Wing\n(Generate Lift)", 
            -200, 200,
            420, 230,
            -200, 200,
            true, 1.0,
            false},
            
        {"Leading Edge Slats\n(Increase Lift)", 
            -200, 300,
            245, 325,
            -200, 300,
            true, 1.0,
            false},

        {"Aileron\n(Change Roll)", 
            windowWidth + 200, 150,
            620, 375,
            windowWidth + 200, 150,
            true, 1.0,
            false},

        {"Spoiler\n(Air breaks/Speed Breaks)", 
            windowWidth + 200, 250,
            600, 435,
            windowWidth + 200, 250,
            true, 1.0,
            false},

        {"Trailing Edge Flaps\n(Increase Lift and Drag)", 
            windowWidth + 200, 400,
            520, 565,
            windowWidth + 200, 400,
            true, 1.0,
            false},

        // For Png 2
        {"Vertical Stabilizer\n(Control Yaw)", 
            windowWidth + 200, 90,
            245, 30,
            windowWidth + 200, 90,
            true, 1.0,
            false},

        {"Rudder\n(Chnage Yaw)", 
            windowWidth + 200, 140,
            325, 70,
            windowWidth + 200, 140,
            true, 1.0,
            false},

        {"Elevator\n(Change Pitch)", 
            -200, 200,
            15, 230,
            -200, 200,
            true, 1.0,
            false},

        {"Horizontal Stabilizer\n(Control Pitch)", 
            -200, 220,
            25, 285,
            -200, 220,
            true, 1.0,
            false}
    }; // <- Make sure to close the vector initialization here.

    double timeElapsed = 0;
    const double delayBetweenLabels = 0.5;

    while(FsCheckWindowOpen()) {
        FsPollDevice();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // Draw main wing image (scaled down)
        int imageX = (windowWidth - png.wid) / 2 + 100;
        int imageY = (windowHeight - png.hei) / 2 + 40;
        float scale1 = 0.86f;  // Adjust this value to resize main image (0.8 = 80% of original size)
        glPixelZoom(scale1, scale1);
        glRasterPos2i(imageX, imageY + png.hei);
        glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);

        // Draw second image in top-left corner
        float scale2 = 0.55f;  // Adjust this value to resize second image (0.5 = 50% of original size)
        glPixelZoom(scale2, scale2);
        glRasterPos2i(10, windowHeight - 325);  // Adjust these values to position the image
        glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);

        // Reset pixel zoom for normal rendering
        glPixelZoom(1.0f, 1.0f);

        // Update and draw labels
        timeElapsed += 0.025;
        
        glColor3ub(0, 0, 0);
        for(size_t i = 0; i < labels.size(); ++i) {
            Label& label = labels[i];
            
            if(timeElapsed > i * delayBetweenLabels && label.isAnimating) {
                double dx = label.targetX - label.currentX;
                double dy = label.targetY - label.currentY;
                double distance = sqrt(dx*dx + dy*dy);
                
                if(distance > 1.0) {
                    label.currentX += (dx * label.speed * 0.05);
                    label.currentY += (dy * label.speed * 0.05);
                } else {
                    label.currentX = label.targetX;
                    label.currentY = label.targetY;
                    label.isAnimating = false;
                }
            }

            std::string line;
            size_t pos = 0;
            size_t lineStart = 0;
            while((pos = label.text.find('\n', lineStart)) != std::string::npos) {
                line = label.text.substr(lineStart, pos - lineStart);
                DrawBoldText(line.c_str(), 
                           label.currentX, 
                           label.currentY + (lineStart == 0 ? 0 : 15),
                           label.useSmallFont);
                lineStart = pos + 1;
            }
            line = label.text.substr(lineStart);
            DrawBoldText(line.c_str(), 
                        label.currentX, 
                        label.currentY + (lineStart == 0 ? 0 : 15),
                        label.useSmallFont);
        }

        FsSwapBuffers();
        FsSleep(20);
    }

    return 0;
}

