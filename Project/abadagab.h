#ifndef ABADAGAB_H
#define ABADAGAB_H

/* 
Overall Instructions:
The class Slide should not be modified at all.
Each of your slides will be similar to Slide_andrewID1 or Slide_andrewID2 (feel free to edit these). Please rename the classes according to your actual andrewID
For example if your andrew ID is acar, your slide classes should be named Slide_acar1, Slide_acar2, Slide_acar3 and so on.
The classes Slide_andrewID1 and Slide_andrewID2 should be modified according to the following rules:
        Anything in Private can be changed. New functions/variables should be added here, not to public. 
        RunSlide():    
            RunSlide() will essentially serve as a substitute for the typical main function. Do not modify the arguments or Return type of this function. All the animation logic will happen here. This is the only place where you'll have the "while (FsCheckWindowOpen() && slideRunning)"
            The global slide controls should not be changed. The spacebar will be used to pause, don't change that either. You are free to change what happens to the animations on pausing/unpausing.

        Slide_andrewID1 is an example of non-interactive animation.
        Slide_andrewID2 is an example of interactive animation.
        
Most of the code comments are on Slide_andrewID1. Slide_andrewID2 is a copy of Slide_andrewID1 with some changes, which are explained in comments.
The main function should only be modified to change the number/names of slides. No other changes.

Right now, the user pause functionality for sound isn't implemented. Don't worry about this, I will make the necessary changes to class Slide (PauseAudio, ResumeAudio) later.
*/


#define M_PI 3.14
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <math.h>
#include "yspng.h"
#include <vector>
#include <string>
#include <cstring>

#include "slide.h"

class abadagab_slide1 : public Slide {
private:
    YsRawPngDecoder png;
    bool isPaused = false;


    double currentArrowLength = 0.0;
    double maxArrowLength = 0.0;
    bool isArrowAnimating = true;
    int posx = 100;
    bool movingRight = true;


public:
    void Reset() {
    }

    abadagab_slide1() : Slide() {

        if (YSOK != png.Decode("abadagab/controls/aircraft_controls.png")) {  // Image credits: https://www.lavionnaire.fr/AngFlightControl.php
            std::cerr << "Error: Could not load down.png" << std::endl;
        }
        //png.Flip();
    }

    SlideTransition RunSlide() override {
        while (FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int keyCode = FsInkey();

            if (keyCode == FSKEY_SPACE) {
                isPaused = !isPaused;
            }
            if (keyCode == FSKEY_LEFT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                return PREVIOUS;
            }
            else if (keyCode == FSKEY_RIGHT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                return NEXT;
            } else if (keyCode == FSKEY_ESC) {
                return EXIT;
            }


            if (!isPaused) {
                Render();
            }

            FsSwapBuffers();
            FsSleep(10);
        }
        return STAY;
    }

private:
    void Render() {
        //glClearColor(0.565f, 0.933f, 0.565f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        std::vector<std::string> textlines = {
            "",
            "LEARN MORE ABOUT AIRCRAFT CONTROLS",
            "IN THE NEXT SLIDE"
        };

        DrawTextBox(posx, 500, 500, 100, textlines, 800, movingRight);
        DrawResizedPngImage(png, 200, 100, 1, 1);
    }



    void DrawTextBox(int& posX, int posY, int width, int height, const std::vector<std::string>& lines, int windowWidth, bool& movingRight) {
        if (movingRight) {
            posX += 2;
            if (posX + width >= windowWidth) {
                movingRight = false;
            }
        }
        else {
            posX -= 2;
            if (posX <= 0) {
                movingRight = true;
            }
        }

        glColor3ub(255, 255, 0);
        glBegin(GL_QUADS);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();


        glColor3ub(0, 0, 0);
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(255, 0, 0);
        int lineHeight = 20;
        int textPosY = posY + lineHeight;

        for (const auto& line : lines) {
            if (textPosY < posY + height - lineHeight) {
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        glRasterPos2i(posX + 10 + dx, textPosY + dy);
                        YsGlDrawFontBitmap12x16(line.c_str());
                    }
                }
                textPosY += lineHeight;
            }
        }
    }



    void DrawResizedPngImage(YsRawPngDecoder& png, int posX, int posY, double scaleX, double scaleY) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int y = 0; y < png.hei; ++y) {
            for (int x = 0; x < png.wid; ++x) {
                int screenX = static_cast<int>(posX + x * scaleX);
                int screenY = static_cast<int>(posY + y * scaleY);

                int idx = (y * png.wid + x) * 4;
                unsigned char r = png.rgba[idx];
                unsigned char g = png.rgba[idx + 1];
                unsigned char b = png.rgba[idx + 2];
                unsigned char a = png.rgba[idx + 3];

                glColor4ub(r, g, b, a);
                glBegin(GL_POINTS);
                glVertex2i(screenX, screenY);
                glEnd();
            }
        }

        glDisable(GL_BLEND);
    }
};



class abadagab_slide2 : public Slide {
private:

    YsSoundPlayer::SoundData Audio1;

    bool isPaused = false;
    bool Question = false;
    std::chrono::steady_clock::time_point startTime;
    double initialVerticalOffset = 0.4;

    double currentArrowLength = 0.0;
    double maxArrowLength = 0.0;
    bool isArrowAnimating = true;


public:
    void Reset() {
        startTime = std::chrono::steady_clock::now();
        isPaused = false;
        currentArrowLength = 0.0;
        isArrowAnimating = true;
        initialVerticalOffset = 0.4;
    }

    abadagab_slide2() : Slide() {
        PrepAudio(Audio1, "abadagab/ailerons/ailerons.wav");
        startTime = std::chrono::steady_clock::now();
    }

    SlideTransition RunSlide() override {
        PlayBGMAudio(Audio1);
        while (FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int keyCode = FsInkey();

            if (keyCode == FSKEY_SPACE) {
                    isPaused = !isPaused;
                if(isPaused) {
                    PauseAudio(Audio1);  
                } else {
                    ResumeAudio(Audio1);  
                }
            }
            if (keyCode == FSKEY_LEFT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return PREVIOUS;
            }
            else if (keyCode == FSKEY_RIGHT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return NEXT;
            } else if (keyCode == FSKEY_ESC) {
                return EXIT;
            }


            if (!isPaused) {
                Render();
            }

            FsSwapBuffers();
            FsSleep(10);
        }
        return STAY;
    }

private:
    void Render() {
        double elapsedTime = GetElapsedTime(startTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawHeading();
        DrawContent1();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double yOffset = 100.0;

        glPushMatrix();
        glTranslated(0, yOffset, 0);
        double verticalOffset;

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /*if (elapsedTime <= streamDisapper)
        {*/
        DisplayImageSequence("abadagab/ailerons/", 0, 333, 0.8, 0.8, 11);
        //}


        std::vector<std::string> leftaileron;
        leftaileron = { "Left Aileron" };


        DrawExtendingArrow(350, 500, 410, 375, 5); // for left arrow
        DrawExtendingArrow(165, 400, 195, 375, 5);

        std::vector<std::string> rightaileron;
        rightaileron = { "Right Aileron" };

        DrawTextBox(350, 405, 125, 50, leftaileron);
        DrawTextBox(200, 350, 125, 50, rightaileron);

        DrawContent2();
    }


    void DrawHeading() {
        const char* heading = "Flight Controls: Ailerons";
        int headingWidth = strlen(heading) * 12;
        int headingX = (800 - headingWidth) / 2;
        int headingY = 50;

        glColor3ub(0, 0, 0);
        glRasterPos2i(headingX, headingY);
        YsGlDrawFontBitmap12x16(heading);
    }

    void DrawContent1() {
        // textbox dimensions
        int rectX = 50;
        int rectY = 70;
        int rectWidth = 725;
        int rectHeight = 260;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 20;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Ailerons: Hinged control surfaces on the trailing edge of each wing, controlling roll",
            "  (left or right tilt) along the aircraft's longitudinal axis.",
            "",
            "* Operation:",
            "  - Pilots turn the yoke or control stick, moving ailerons in opposite directions",
            "    to adjust lift on each wing.",
            "",
            "* Roll Control:",
            "  - This differential lift causes the aircraft to roll, aiding in turns and maneuvering.",
            "",
            "* Efficiency:",
            "  - Designs like differential and frise ailerons reduce adverse yaw, enhancing control.",
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawContent2() {
        // textbox dimensions
        int rectX = 495;
        int rectY = 400;
        int rectWidth = 225;
        int rectHeight = 100;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 10;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Left Aileron = up;",
            "Aircraft roll = right",
            "* Right Aileron = up;",
            "Aircraft roll = left"
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawExtendingArrow(int startX, int startY, int targetX, int targetY, double speed) {
        double angle = atan2(targetY - startY, targetX - startX);
        maxArrowLength = sqrt((targetX - startX) * (targetX - startX) + (targetY - startY) * (targetY - startY));
        if (isArrowAnimating && currentArrowLength < maxArrowLength) {
            currentArrowLength += speed;
            if (currentArrowLength > maxArrowLength) {
                currentArrowLength = maxArrowLength;
                isArrowAnimating = false;
            }
        }
        int endX = startX + static_cast<int>(currentArrowLength * cos(angle));
        int endY = startY + static_cast<int>(currentArrowLength * sin(angle));
        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2i(startX, startY);
        glVertex2i(endX, endY);
        glEnd();

        int headLength = 10;
        double headAngle = M_PI / 6;
        int headX1 = endX + headLength * cos(angle + M_PI - headAngle);
        int headY1 = endY + headLength * sin(angle + M_PI - headAngle);
        int headX2 = endX + headLength * cos(angle + M_PI + headAngle);
        int headY2 = endY + headLength * sin(angle + M_PI + headAngle);

        glBegin(GL_TRIANGLES);
        glVertex2i(endX, endY);
        glVertex2i(headX1, headY1);
        glVertex2i(headX2, headY2);
        glEnd();
    }

    //void DrawOvalTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
    //    const int numSegments = 100;
    //    glColor3ub(255, 255, 0);
    //    glBegin(GL_TRIANGLE_FAN);
    //    glVertex2i(centerX, centerY);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();

    //    glColor3ub(255, 0, 0);  // Red border color
    //    glLineWidth(2.0f);
    //    glBegin(GL_LINE_LOOP);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();
    //    glColor3ub(0, 0, 0);
    //    int lineHeight = 20;
    //    int totalTextHeight = lines.size() * lineHeight;
    //    int startY = centerY - totalTextHeight / 2 + lineHeight / 2;

    //    for (size_t i = 0; i < lines.size(); ++i) {
    //        int textPosX = centerX - (lines[i].length() * 8) / 2;
    //        int textPosY = startY + i * lineHeight;

    //        glRasterPos2i(textPosX, textPosY);
    //        YsGlDrawFontBitmap12x16(lines[i].c_str());
    //    }
    //}

    void DrawTextBox(int posX, int posY, int width, int height, const std::vector<std::string>& lines) {
        glColor3ub(173, 216, 230);
        glBegin(GL_QUADS);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        int lineHeight = 20;
        int textPosY = posY + lineHeight;

        for (const auto& line : lines) {
            if (textPosY < posY + height - lineHeight) {
                glRasterPos2i(posX + 10, textPosY);
                YsGlDrawFontBitmap8x12(line.c_str());
                textPosY += lineHeight;
            }
        }
    }


    void DrawResizedPngImage(YsRawPngDecoder& png, int posX, int posY, double scaleX, double scaleY) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int y = 0; y < png.hei; ++y) {
            for (int x = 0; x < png.wid; ++x) {
                int screenX = static_cast<int>(posX + x * scaleX);
                int screenY = static_cast<int>(posY + y * scaleY);

                int idx = (y * png.wid + x) * 4;
                unsigned char r = png.rgba[idx];
                unsigned char g = png.rgba[idx + 1];
                unsigned char b = png.rgba[idx + 2];
                unsigned char a = png.rgba[idx + 3];

                glColor4ub(r, g, b, a);
                glBegin(GL_POINTS);
                glVertex2i(screenX, screenY);
                glEnd();
            }
        }

        glDisable(GL_BLEND);
    }


    double GetElapsedTime(const std::chrono::steady_clock::time_point& startTime) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        return duration.count() / 1000.0;
    }

    void DisplayImageSequence(const std::string& basePath, int posX, int posY, double scaleX, double scaleY, int totalFrames) {
        YsRawPngDecoder png;
        //png.Flip();
        const double frameDuration = 0.1;
        double elapsedTime = GetElapsedTime(startTime);
        int currentFrame = static_cast<int>(elapsedTime / frameDuration) % totalFrames;
        char filename[256];
        snprintf(filename, sizeof(filename), "%sframe_%02d_delay-0.1s.png", basePath.c_str(), currentFrame + 1);

        if (YSOK == png.Decode(filename)) {
            //png.Flip();
            DrawResizedPngImage(png, posX, posY, scaleX, scaleY);
        }
        else {
            std::cerr << "Error: Could not load " << filename << std::endl;
        }
    }
};

class abadagab_slide3 : public Slide {
private:

    YsSoundPlayer::SoundData Audio1;

    bool isPaused = false;
    bool Question = false;
    std::chrono::steady_clock::time_point startTime;
    double initialVerticalOffset = 0.4;

    double currentArrowLength = 0.0;
    double maxArrowLength = 0.0;
    bool isArrowAnimating = true;


public:
    void Reset() {
        startTime = std::chrono::steady_clock::now();
        isPaused = false;
        currentArrowLength = 0.0;
        isArrowAnimating = true;
        initialVerticalOffset = 0.4;
    }

    abadagab_slide3() : Slide() {
        PrepAudio(Audio1, "abadagab/elevators/elevators.wav");
        startTime = std::chrono::steady_clock::now();
    }

    SlideTransition RunSlide() override {
        PlayBGMAudio(Audio1);
        while (FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int keyCode = FsInkey();

            if (keyCode == FSKEY_SPACE) {
                    isPaused = !isPaused;
                if(isPaused) {
                    PauseAudio(Audio1);  
                } else {
                    ResumeAudio(Audio1);  
                }
            }
            if (keyCode == FSKEY_LEFT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return PREVIOUS;
            }
            else if (keyCode == FSKEY_RIGHT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return NEXT;
            } else if (keyCode == FSKEY_ESC) {
                return EXIT;
            }


            if (!isPaused) {
                Render();
            }

            FsSwapBuffers();
            FsSleep(10);
        }
        return STAY;
    }

private:
    void Render() {
        double elapsedTime = GetElapsedTime(startTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawHeading();
        DrawContent1();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double yOffset = 100.0;

        glPushMatrix();
        glTranslated(0, yOffset, 0);
        double verticalOffset;

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /*if (elapsedTime <= streamDisapper)
        {*/
        DisplayImageSequence("abadagab/elevators/", 0, 380, 0.7, 0.7, 11);
        //}


        std::vector<std::string> elev;
        elev = { "Elevators" };


        DrawExtendingArrow(300, 450, 345, 420, 5);
        DrawExtendingArrow(255, 425, 355, 420, 5);

        DrawTextBox(350, 405, 125, 50, elev);

        DrawContent2();
    }


    void DrawHeading() {
        const char* heading = "Flight Controls: Elevators";
        int headingWidth = strlen(heading) * 12;
        int headingX = (800 - headingWidth) / 2;
        int headingY = 50;

        glColor3ub(0, 0, 0);
        glRasterPos2i(headingX, headingY);
        YsGlDrawFontBitmap12x16(heading);
    }

    void DrawContent1() {
        // textbox dimensions
        int rectX = 50;
        int rectY = 70;
        int rectWidth = 725;
        int rectHeight = 280;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 20;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
    "* Elevators: Hinged control surfaces on the trailing edge of the horizontal stabilizer,",
    "  controlling pitch (nose up or down) along the aircraft's lateral axis.",
    "",
    "* Operation:",
    "  - Pilots push or pull the yoke or control stick, moving elevators up or down",
    "    to change the angle of attack of the wings.",
    "",
    "* Pitch Control:",
    "  - Adjusting the elevators changes the aircraft's pitch, allowing it to climb, descend,",
    "    or maintain level flight.",
    "",
    "* Stability:",
    "  - Properly designed elevators ensure smooth pitch control and stability during flight.",
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawContent2() {
        // textbox dimensions
        int rectX = 495;
        int rectY = 400;
        int rectWidth = 225;
        int rectHeight = 100;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 10;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Elevators = up;",
            "Aircraft pitch = down",
            "* Elevators = down;",
            "Aircraft pitch = up"
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawExtendingArrow(int startX, int startY, int targetX, int targetY, double speed) {
        double angle = atan2(targetY - startY, targetX - startX);
        maxArrowLength = sqrt((targetX - startX) * (targetX - startX) + (targetY - startY) * (targetY - startY));
        if (isArrowAnimating && currentArrowLength < maxArrowLength) {
            currentArrowLength += speed;
            if (currentArrowLength > maxArrowLength) {
                currentArrowLength = maxArrowLength;
                isArrowAnimating = false;
            }
        }
        int endX = startX + static_cast<int>(currentArrowLength * cos(angle));
        int endY = startY + static_cast<int>(currentArrowLength * sin(angle));
        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2i(startX, startY);
        glVertex2i(endX, endY);
        glEnd();

        int headLength = 10;
        double headAngle = M_PI / 6;
        int headX1 = endX + headLength * cos(angle + M_PI - headAngle);
        int headY1 = endY + headLength * sin(angle + M_PI - headAngle);
        int headX2 = endX + headLength * cos(angle + M_PI + headAngle);
        int headY2 = endY + headLength * sin(angle + M_PI + headAngle);

        glBegin(GL_TRIANGLES);
        glVertex2i(endX, endY);
        glVertex2i(headX1, headY1);
        glVertex2i(headX2, headY2);
        glEnd();
    }

    //void DrawOvalTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
    //    const int numSegments = 100;
    //    glColor3ub(255, 255, 0);
    //    glBegin(GL_TRIANGLE_FAN);
    //    glVertex2i(centerX, centerY);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();

    //    glColor3ub(255, 0, 0);  // Red border color
    //    glLineWidth(2.0f);
    //    glBegin(GL_LINE_LOOP);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();
    //    glColor3ub(0, 0, 0);
    //    int lineHeight = 20;
    //    int totalTextHeight = lines.size() * lineHeight;
    //    int startY = centerY - totalTextHeight / 2 + lineHeight / 2;

    //    for (size_t i = 0; i < lines.size(); ++i) {
    //        int textPosX = centerX - (lines[i].length() * 8) / 2;
    //        int textPosY = startY + i * lineHeight;

    //        glRasterPos2i(textPosX, textPosY);
    //        YsGlDrawFontBitmap12x16(lines[i].c_str());
    //    }
    //}

    void DrawTextBox(int posX, int posY, int width, int height, const std::vector<std::string>& lines) {
        glColor3ub(173, 216, 230);
        glBegin(GL_QUADS);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        int lineHeight = 20;
        int textPosY = posY + lineHeight;

        for (const auto& line : lines) {
            if (textPosY < posY + height - lineHeight) {
                glRasterPos2i(posX + 10, textPosY);
                YsGlDrawFontBitmap8x12(line.c_str());
                textPosY += lineHeight;
            }
        }
    }


    void DrawResizedPngImage(YsRawPngDecoder& png, int posX, int posY, double scaleX, double scaleY) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int y = 0; y < png.hei; ++y) {
            for (int x = 0; x < png.wid; ++x) {
                int screenX = static_cast<int>(posX + x * scaleX);
                int screenY = static_cast<int>(posY + y * scaleY);

                int idx = (y * png.wid + x) * 4;
                unsigned char r = png.rgba[idx];
                unsigned char g = png.rgba[idx + 1];
                unsigned char b = png.rgba[idx + 2];
                unsigned char a = png.rgba[idx + 3];

                glColor4ub(r, g, b, a);
                glBegin(GL_POINTS);
                glVertex2i(screenX, screenY);
                glEnd();
            }
        }

        glDisable(GL_BLEND);
    }


    double GetElapsedTime(const std::chrono::steady_clock::time_point& startTime) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        return duration.count() / 1000.0;
    }

    void DisplayImageSequence(const std::string& basePath, int posX, int posY, double scaleX, double scaleY, int totalFrames) {
        YsRawPngDecoder png;
        //png.Flip();
        const double frameDuration = 0.1;
        double elapsedTime = GetElapsedTime(startTime);
        int currentFrame = static_cast<int>(elapsedTime / frameDuration) % totalFrames;
        char filename[256];
        snprintf(filename, sizeof(filename), "%sframe_%02d_delay-0.1s.png", basePath.c_str(), currentFrame + 1);

        if (YSOK == png.Decode(filename)) {
            //png.Flip();
            DrawResizedPngImage(png, posX, posY, scaleX, scaleY);
        }
        else {
            std::cerr << "Error: Could not load " << filename << std::endl;
        }
    }
};


class abadagab_slide4 : public Slide {
private:

    YsSoundPlayer::SoundData Audio1;

    bool isPaused = false;
    bool Question = false;
    std::chrono::steady_clock::time_point startTime;
    double initialVerticalOffset = 0.4;

    double currentArrowLength = 0.0;
    double maxArrowLength = 0.0;
    bool isArrowAnimating = true;


public:
    void Reset() {
        startTime = std::chrono::steady_clock::now();
        isPaused = false;
        currentArrowLength = 0.0;
        isArrowAnimating = true;
        initialVerticalOffset = 0.4;
    }

    abadagab_slide4() : Slide() {
        PrepAudio(Audio1, "abadagab/rudder/rudder.wav");
        startTime = std::chrono::steady_clock::now();
    }

    SlideTransition RunSlide() override {
        PlayBGMAudio(Audio1);
        while (FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int keyCode = FsInkey();

            if (keyCode == FSKEY_SPACE) {
                    isPaused = !isPaused;
                if(isPaused) {
                    PauseAudio(Audio1);  
                } else {
                    ResumeAudio(Audio1);  
                }
            }
            if (keyCode == FSKEY_LEFT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return PREVIOUS;
            }
            else if (keyCode == FSKEY_RIGHT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return NEXT;
            } else if (keyCode == FSKEY_ESC) {
                return EXIT;
            }


            if (!isPaused) {
                Render();
            }

            FsSwapBuffers();
            FsSleep(10);
        }
        return STAY;
    }

private:
    void Render() {
        double elapsedTime = GetElapsedTime(startTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawHeading();
        DrawContent1();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double yOffset = 100.0;

        glPushMatrix();
        glTranslated(0, yOffset, 0);
        double verticalOffset;

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /*if (elapsedTime <= streamDisapper)
        {*/
        DisplayImageSequence("abadagab/rudder/", 0, 380, 0.7, 0.7, 11);
        //}


        std::vector<std::string> rudd;
        rudd = { "Rudder" };


        DrawExtendingArrow(300, 430, 345, 420, 5);

        DrawTextBox(350, 405, 125, 50, rudd);

        DrawContent2();
    }


    void DrawHeading() {
        const char* heading = "Flight Controls: Rudder";
        int headingWidth = strlen(heading) * 12;
        int headingX = (800 - headingWidth) / 2;
        int headingY = 50;

        glColor3ub(0, 0, 0);
        glRasterPos2i(headingX, headingY);
        YsGlDrawFontBitmap12x16(heading);
    }

    void DrawContent1() {
        // textbox dimensions
        int rectX = 50;
        int rectY = 70;
        int rectWidth = 725;
        int rectHeight = 295;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 20;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
    "* Rudder: Hinged control surface on the trailing edge of the vertical stabilizer,",
    "  controlling yaw (nose left or right) along the aircraft's vertical axis.",
    "",
    "* Operation:",
    "  - Pilots use the rudder pedals to move the rudder left or right, directing airflow",
    "    and altering the yaw.",
    "",
    "* Yaw Control:",
    "  - Adjusting the rudder helps align the aircraft with the desired flight path,",
    "    particularly during turns and crosswind landings.",
    "",
    "* Coordination:",
    "  - Used in conjunction with ailerons and elevators for coordinated flight,",
    "    minimizing side-slip and maintaining balance.",
        };


        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawContent2() {
        // textbox dimensions
        int rectX = 495;
        int rectY = 400;
        int rectWidth = 225;
        int rectHeight = 100;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 10;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Rudder = left;",
            "Aircraft yaw = left",
            "* Rudder = right;",
            "Aircraft yaw = right"
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawExtendingArrow(int startX, int startY, int targetX, int targetY, double speed) {
        double angle = atan2(targetY - startY, targetX - startX);
        maxArrowLength = sqrt((targetX - startX) * (targetX - startX) + (targetY - startY) * (targetY - startY));
        if (isArrowAnimating && currentArrowLength < maxArrowLength) {
            currentArrowLength += speed;
            if (currentArrowLength > maxArrowLength) {
                currentArrowLength = maxArrowLength;
                isArrowAnimating = false;
            }
        }
        int endX = startX + static_cast<int>(currentArrowLength * cos(angle));
        int endY = startY + static_cast<int>(currentArrowLength * sin(angle));
        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2i(startX, startY);
        glVertex2i(endX, endY);
        glEnd();

        int headLength = 10;
        double headAngle = M_PI / 6;
        int headX1 = endX + headLength * cos(angle + M_PI - headAngle);
        int headY1 = endY + headLength * sin(angle + M_PI - headAngle);
        int headX2 = endX + headLength * cos(angle + M_PI + headAngle);
        int headY2 = endY + headLength * sin(angle + M_PI + headAngle);

        glBegin(GL_TRIANGLES);
        glVertex2i(endX, endY);
        glVertex2i(headX1, headY1);
        glVertex2i(headX2, headY2);
        glEnd();
    }

    //void DrawOvalTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
    //    const int numSegments = 100;
    //    glColor3ub(255, 255, 0);
    //    glBegin(GL_TRIANGLE_FAN);
    //    glVertex2i(centerX, centerY);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();

    //    glColor3ub(255, 0, 0);  // Red border color
    //    glLineWidth(2.0f);
    //    glBegin(GL_LINE_LOOP);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();
    //    glColor3ub(0, 0, 0);
    //    int lineHeight = 20;
    //    int totalTextHeight = lines.size() * lineHeight;
    //    int startY = centerY - totalTextHeight / 2 + lineHeight / 2;

    //    for (size_t i = 0; i < lines.size(); ++i) {
    //        int textPosX = centerX - (lines[i].length() * 8) / 2;
    //        int textPosY = startY + i * lineHeight;

    //        glRasterPos2i(textPosX, textPosY);
    //        YsGlDrawFontBitmap12x16(lines[i].c_str());
    //    }
    //}

    void DrawTextBox(int posX, int posY, int width, int height, const std::vector<std::string>& lines) {
        glColor3ub(173, 216, 230);
        glBegin(GL_QUADS);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        int lineHeight = 20;
        int textPosY = posY + lineHeight;

        for (const auto& line : lines) {
            if (textPosY < posY + height - lineHeight) {
                glRasterPos2i(posX + 10, textPosY);
                YsGlDrawFontBitmap8x12(line.c_str());
                textPosY += lineHeight;
            }
        }
    }


    void DrawResizedPngImage(YsRawPngDecoder& png, int posX, int posY, double scaleX, double scaleY) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int y = 0; y < png.hei; ++y) {
            for (int x = 0; x < png.wid; ++x) {
                int screenX = static_cast<int>(posX + x * scaleX);
                int screenY = static_cast<int>(posY + y * scaleY);

                int idx = (y * png.wid + x) * 4;
                unsigned char r = png.rgba[idx];
                unsigned char g = png.rgba[idx + 1];
                unsigned char b = png.rgba[idx + 2];
                unsigned char a = png.rgba[idx + 3];

                glColor4ub(r, g, b, a);
                glBegin(GL_POINTS);
                glVertex2i(screenX, screenY);
                glEnd();
            }
        }

        glDisable(GL_BLEND);
    }


    double GetElapsedTime(const std::chrono::steady_clock::time_point& startTime) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        return duration.count() / 1000.0;
    }

    void DisplayImageSequence(const std::string& basePath, int posX, int posY, double scaleX, double scaleY, int totalFrames) {
        YsRawPngDecoder png;
        //png.Flip();
        const double frameDuration = 0.1;
        double elapsedTime = GetElapsedTime(startTime);
        int currentFrame = static_cast<int>(elapsedTime / frameDuration) % totalFrames;
        char filename[256];
        snprintf(filename, sizeof(filename), "%sframe_%02d_delay-0.1s.png", basePath.c_str(), currentFrame + 1);

        if (YSOK == png.Decode(filename)) {
            //png.Flip();
            DrawResizedPngImage(png, posX, posY, scaleX, scaleY);
        }
        else {
            std::cerr << "Error: Could not load " << filename << std::endl;
        }
    }
};


class abadagab_slide5 : public Slide {
private:

    YsSoundPlayer::SoundData Audio1;

    bool isPaused = false;
    bool Question = false;
    std::chrono::steady_clock::time_point startTime;
    double initialVerticalOffset = 0.4;

    double currentArrowLength = 0.0;
    double maxArrowLength = 0.0;
    bool isArrowAnimating = true;


public:
    void Reset() {
        startTime = std::chrono::steady_clock::now();
        isPaused = false;
        currentArrowLength = 0.0;
        isArrowAnimating = true;
        initialVerticalOffset = 0.4;
    }

    abadagab_slide5() : Slide() {
        PrepAudio(Audio1, "abadagab/flaps/flaps.wav");
        startTime = std::chrono::steady_clock::now();
    }

    SlideTransition RunSlide() override {
        PlayBGMAudio(Audio1);
        while (FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int keyCode = FsInkey();

            if (keyCode == FSKEY_SPACE) {
                    isPaused = !isPaused;
                if(isPaused) {
                    PauseAudio(Audio1);  
                } else {
                    ResumeAudio(Audio1);  
                }
            }
            if (keyCode == FSKEY_LEFT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return PREVIOUS;
            }
            else if (keyCode == FSKEY_RIGHT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
                initialVerticalOffset = 0.4;
                return NEXT;
            }
             else if (keyCode == FSKEY_ESC) {
                return EXIT;
            }


            if (!isPaused) {
                Render();
            }

            FsSwapBuffers();
            FsSleep(10);
        }
        return STAY;
    }

private:
    void Render() {
        double elapsedTime = GetElapsedTime(startTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawHeading();
        DrawContent1();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double yOffset = 100.0;

        glPushMatrix();
        glTranslated(0, yOffset, 0);
        double verticalOffset;

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /*if (elapsedTime <= streamDisapper)
        {*/
        DisplayImageSequence("abadagab/flaps/", 50, 450, 1, 1, 15);
        //}

        DrawContent2();
    }


    void DrawHeading() {
        const char* heading = "Flight Controls: Flaps";
        int headingWidth = strlen(heading) * 12;
        int headingX = (800 - headingWidth) / 2;
        int headingY = 50;

        glColor3ub(0, 0, 0);
        glRasterPos2i(headingX, headingY);
        YsGlDrawFontBitmap12x16(heading);
    }

    void DrawContent1() {
        // textbox dimensions
        int rectX = 50;
        int rectY = 70;
        int rectWidth = 725;
        int rectHeight = 295;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 20;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
    "* Flaps: Hinged control surfaces located on the trailing edge of the wings,",
    "  enhancing lift and drag for improved control during takeoff and landing.",
    "",
    "* Operation:",
    "  - Pilots extend or retract flaps using a lever or switch in the cockpit,",
    "    adjusting the wing's shape and increasing its surface area.",
    "",
    "* Lift and Drag Control:",
    "  - Extended flaps increase lift at lower speeds, allowing for shorter takeoffs",
    "    and slower, controlled landings.",
    "",
    "* Maneuverability:",
    "  - Flaps enable steeper descent angles without increasing airspeed,",
    "    aiding in precision landings.",
        };


        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawContent2() {
        // textbox dimensions
        int rectX = 495;
        int rectY = 400;
        int rectWidth = 225;
        int rectHeight = 100;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 10;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Flaps = Extended for",
            "Aircraft Takeoff and",
            "Landing",
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1);
    }

    void DrawExtendingArrow(int startX, int startY, int targetX, int targetY, double speed) {
        double angle = atan2(targetY - startY, targetX - startX);
        maxArrowLength = sqrt((targetX - startX) * (targetX - startX) + (targetY - startY) * (targetY - startY));
        if (isArrowAnimating && currentArrowLength < maxArrowLength) {
            currentArrowLength += speed;
            if (currentArrowLength > maxArrowLength) {
                currentArrowLength = maxArrowLength;
                isArrowAnimating = false;
            }
        }
        int endX = startX + static_cast<int>(currentArrowLength * cos(angle));
        int endY = startY + static_cast<int>(currentArrowLength * sin(angle));
        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2i(startX, startY);
        glVertex2i(endX, endY);
        glEnd();

        int headLength = 10;
        double headAngle = M_PI / 6;
        int headX1 = endX + headLength * cos(angle + M_PI - headAngle);
        int headY1 = endY + headLength * sin(angle + M_PI - headAngle);
        int headX2 = endX + headLength * cos(angle + M_PI + headAngle);
        int headY2 = endY + headLength * sin(angle + M_PI + headAngle);

        glBegin(GL_TRIANGLES);
        glVertex2i(endX, endY);
        glVertex2i(headX1, headY1);
        glVertex2i(headX2, headY2);
        glEnd();
    }

    //void DrawOvalTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
    //    const int numSegments = 100;
    //    glColor3ub(255, 255, 0);
    //    glBegin(GL_TRIANGLE_FAN);
    //    glVertex2i(centerX, centerY);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();

    //    glColor3ub(255, 0, 0);  // Red border color
    //    glLineWidth(2.0f);
    //    glBegin(GL_LINE_LOOP);

    //    for (int i = 0; i <= numSegments; ++i) {
    //        double angle = 2.0 * M_PI * i / numSegments;
    //        double x = centerX + (width / 2.0) * cos(angle);
    //        double y = centerY + (height / 2.0) * sin(angle);
    //        glVertex2d(x, y);
    //    }
    //    glEnd();
    //    glColor3ub(0, 0, 0);
    //    int lineHeight = 20;
    //    int totalTextHeight = lines.size() * lineHeight;
    //    int startY = centerY - totalTextHeight / 2 + lineHeight / 2;

    //    for (size_t i = 0; i < lines.size(); ++i) {
    //        int textPosX = centerX - (lines[i].length() * 8) / 2;
    //        int textPosY = startY + i * lineHeight;

    //        glRasterPos2i(textPosX, textPosY);
    //        YsGlDrawFontBitmap12x16(lines[i].c_str());
    //    }
    //}

    void DrawTextBox(int posX, int posY, int width, int height, const std::vector<std::string>& lines) {
        glColor3ub(173, 216, 230);
        glBegin(GL_QUADS);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(posX, posY);
        glVertex2i(posX + width, posY);
        glVertex2i(posX + width, posY + height);
        glVertex2i(posX, posY + height);
        glEnd();

        glColor3ub(0, 0, 0);
        int lineHeight = 20;
        int textPosY = posY + lineHeight;

        for (const auto& line : lines) {
            if (textPosY < posY + height - lineHeight) {
                glRasterPos2i(posX + 10, textPosY);
                YsGlDrawFontBitmap8x12(line.c_str());
                textPosY += lineHeight;
            }
        }
    }


    void DrawResizedPngImage(YsRawPngDecoder& png, int posX, int posY, double scaleX, double scaleY) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int y = 0; y < png.hei; ++y) {
            for (int x = 0; x < png.wid; ++x) {
                int screenX = static_cast<int>(posX + x * scaleX);
                int screenY = static_cast<int>(posY + y * scaleY);

                int idx = (y * png.wid + x) * 4;
                unsigned char r = png.rgba[idx];
                unsigned char g = png.rgba[idx + 1];
                unsigned char b = png.rgba[idx + 2];
                unsigned char a = png.rgba[idx + 3];

                glColor4ub(r, g, b, a);
                glBegin(GL_POINTS);
                glVertex2i(screenX, screenY);
                glEnd();
            }
        }

        glDisable(GL_BLEND);
    }


    double GetElapsedTime(const std::chrono::steady_clock::time_point& startTime) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        return duration.count() / 1000.0;
    }

    void DisplayImageSequence(const std::string& basePath, int posX, int posY, double scaleX, double scaleY, int totalFrames) {
        YsRawPngDecoder png;
        //png.Flip();
        const double frameDuration = 0.1;
        double elapsedTime = GetElapsedTime(startTime);
        int currentFrame = static_cast<int>(elapsedTime / frameDuration) % totalFrames;
        char filename[256];
        snprintf(filename, sizeof(filename), "%sframe_%02d_delay-0.01s.png", basePath.c_str(), currentFrame + 1);

        if (YSOK == png.Decode(filename)) {
            //png.Flip();
            DrawResizedPngImage(png, posX, posY, scaleX, scaleY);
        }
        else {
            std::cerr << "Error: Could not load " << filename << std::endl;
        }
    }
};

#endif
