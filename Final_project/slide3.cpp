// Slide 3

#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

struct Box {
    std::string title;
    std::vector<std::string> content;
    double x, y, width, height;
    double opacity;
    double targetX;
    bool isActive;
    bool isExiting;
};

struct ImageWithCaption {
    YsRawPngDecoder png;
    std::string caption;
    int x, y;
};

void DrawBoldText(const char* str, double x, double y, bool isLarge) {
    const double offset = 0.5;
    glColor4f(0, 0, 0, 1.0);
    
    for(double dx = -offset; dx <= offset; dx += offset) {
        for(double dy = -offset; dy <= offset; dy += offset) {
            glRasterPos2d(x + dx, y + dy);
            if(isLarge) {
                YsGlDrawFontBitmap16x24(str);
            } else {
                YsGlDrawFontBitmap12x16(str);
            }
        }
    }
}

void DrawBox(Box& box) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0, 0, 0, box.opacity);
    glBegin(GL_LINE_LOOP);
    glVertex2d(box.x, box.y);
    glVertex2d(box.x + box.width, box.y);
    glVertex2d(box.x + box.width, box.y + box.height);
    glVertex2d(box.x, box.y + box.height);
    glEnd();

    const int titleLen = strlen(box.title.c_str()) * 16;
    double titleX = box.x + (box.width - titleLen) / 2;
    glColor4f(0, 0, 0, box.opacity);
    DrawBoldText(box.title.c_str(), titleX, box.y + 30, true);

    double yOffset = 70;  // Reduced from 80 to move content up
    for(const auto& line : box.content) {
        const int lineLen = strlen(line.c_str()) * 12;
        double lineX = box.x + (box.width - lineLen) / 2;
        glColor4f(0, 0, 0, box.opacity);
        DrawBoldText(line.c_str(), lineX, box.y + yOffset, false);
        yOffset += 25;  // Reduced from 30 to compact content
    }

    glDisable(GL_BLEND);
}

void DrawTitle(const char* title, int windowWidth) {
    const int titleLen = strlen(title) * 16;
    DrawBoldText(title, (windowWidth - titleLen) / 2, 50, true);
}

void DrawScaledImage(const YsRawPngDecoder& png, int x, int y, int targetWidth, int targetHeight) {
    // Calculate scaling factors
    float scaleX = (float)targetWidth / png.wid;
    float scaleY = (float)targetHeight / png.hei;
    
    // Use uniform scaling to maintain aspect ratio
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    
    int finalWidth = png.wid * scale;
    int finalHeight = png.hei * scale;
    
    // Center the image horizontally within its allocated space
    int offsetX = (targetWidth - finalWidth) / 2;
    
    glRasterPos2i(x + offsetX, y);
    glPixelZoom(scale, scale);  // Set scaling factor
    glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
    glPixelZoom(1.0f, 1.0f);  // Reset scaling factor
}

void DrawImages(const std::vector<ImageWithCaption>& images, int targetWidth, int targetHeight) {
    for(const auto& img : images) {
        // Draw scaled image
        DrawScaledImage(img.png, img.x, img.y, targetWidth, targetHeight);

        // Draw caption
        const int captionLen = strlen(img.caption.c_str()) * 8;
        double captionX = img.x + (targetWidth - captionLen) / 2;
        glRasterPos2d(captionX, img.y + targetHeight -140);
        YsGlDrawFontBitmap8x12(img.caption.c_str());
    }
}


int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 600;
    FsOpenWindow(16, 16, windowWidth, windowHeight, 1);

    // Load images
    std::vector<ImageWithCaption> images(3);
    if(YSOK != images[0].png.Decode("slide3.png") ||
       YSOK != images[1].png.Decode("takeoff.png") ||
       YSOK != images[2].png.Decode("landing.png")) {
        std::cout << "Failed to load PNG images" << std::endl;
        return 1;
    }
    
    for(auto& img : images) {
        img.png.Flip();
    }

    // Set image positions and captions with smaller dimensions
    const int imgWidth = 220;   // Target width for each image
    const int imgHeight = 160;  // Target height for each image
    const int imgSpacing = 30;  // Space between images
    const int totalWidth = imgWidth * 3 + imgSpacing * 2;
    int startX = (windowWidth - totalWidth) / 2;
    int imgY = 500;  // Position below text boxes

    images[0].caption = "wing control surfaces";
    images[1].caption = "Wings while takeoff";
    images[2].caption = "Wings while landing";

    for(int i = 0; i < 3; ++i) {
        images[i].x = startX + i * (imgWidth + imgSpacing);
        images[i].y = imgY;
    }


    // Define boxes with their content
    std::vector<Box> boxes = {
        {
            "Control Surfaces of the Wing",
            {
                "Flaps: Increasing Lift",
                "- Used during takeoff and landing",
                "- Generates extra lift by extending wing shape",
                "",
                "Ailerons: Controlling Roll",
                "- Located on wing tips",
                "- Controls roll for directional changes"
            },
            -500, 90,
            600, 250,
            1.0,
            (windowWidth - 500) / 2,
            true,
            false
        },
        {
            "Lift and Drag Relationship",
            {
                "Wing Adjustments:",
                "- Flaps and ailerons adjust wing shape",
                "- Alters lift-to-drag ratio",
                "- High Lift: Takeoff and landing",
                "- High Drag: Spoilers for descent",
                "",
                "Performance Balance:",
                "- Maintains maneuverability"
            },
            -500, 90,
            600, 250,
            1.0,
            (windowWidth - 500) / 2,
            false,
            false
        },
        {
            "Takeaway Message",
            {
                "Wing control surfaces like flaps and",
                "ailerons manage lift, drag, and",
                "stability, ensuring safe and efficient",
                "flight maneuverability."
            },
            -500, 90,
            500, 200,
            1.0,
            (windowWidth - 500) / 2,
            false,
            false
        }
    };

    int currentBox = 0;
    double timeCounter = 0;
    double moveSpeed = 10.0;
    bool centerReached = false;

    while(FsCheckWindowOpen()) {
        FsPollDevice();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Pure white

        DrawTitle("Wing Control Surfaces and Their Function ", windowWidth);
        timeCounter += 0.025;

        // Handle current box animation
        if(currentBox < boxes.size()) {
            Box& box = boxes[currentBox];
            
            if(!centerReached) {
                if(box.x < box.targetX) {
                    box.x += moveSpeed;
                    if(box.x >= box.targetX) {
                        box.x = box.targetX;
                        centerReached = true;
                        timeCounter = 0;
                    }
                }
            } else if(!box.isExiting && timeCounter >= 5.0) {
                box.isExiting = true;
                timeCounter = 0;
            } else if(box.isExiting) {
                box.x += moveSpeed;
                if(box.x > windowWidth + 100) {  // Added extra distance for full exit
                    currentBox++;
                    centerReached = false;
                    if(currentBox < boxes.size()) {
                        boxes[currentBox].isActive = true;
                    }
                }
            }
        }

        // Draw boxes
        for(auto& box : boxes) {
            if(box.isActive) {
                DrawBox(box);
            }
        }

        // Draw scaled images
        DrawImages(images, imgWidth, imgHeight);

        FsSwapBuffers();
        FsSleep(20);
    }

    return 0;
}

