#ifndef SAISRAVY_H
#define SAISRAVY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <chrono>
#include "fssimplewindow.h"
#include "yspng.h"
#include <cstring>
#include "yssimplesound.h"
#include "ysglfontdata.h"
#include <algorithm>

#include "slide.h"

struct Point {
    double x;
    double y;
};


// Arvinds code starts from here!
// I need to modify this
//--------------------------------------------------------------------------------------


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



/*
This enum declared globally is needed for the main function to run.
It is declared as an enum for the sake of code readability (instead of just using numbers)
DO NOT declare a new enum, append an element onto the end of this one if neccessary
*/








// DO NOT Modify this. You should only be changing Slide_andrewID1 and 2 (and adding more if necessary)

class saisravy_slide1_intro : public Slide {
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

    saisravy_slide1_intro() : Slide() {

        if (YSOK != png.Decode("saisravy/bernoulli/liftintro.png")) {
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
        //glClearColor(0.565f, 0.933f, 0.565f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        std::vector<std::string> textlines = {
            "",
            "LEARN MORE ABOUT LIFT",
            "IN THE NEXT SLIDE"
        };
        
        DrawTextBox(posx, 500, 300, 100, textlines, 800, movingRight);
        DrawResizedPngImage(png, 100, 100, 1, 1);
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



// saisravy_slide1 : Talks about basics of bernoullis principle
//my slide1

class saisravy_slide1 : public Slide {
private:
    std::vector<Point> airfoil;
    YsRawPngDecoder png;
    YsRawPngDecoder png_flip;
    YsRawPngDecoder arrows_bernoulli;
    YsRawPngDecoder arrow2;
    YsRawPngDecoder highpressure1;
    YsRawPngDecoder highpressure2;
    YsRawPngDecoder lowpressure1;
    YsRawPngDecoder lowpressure2;
    YsRawPngDecoder Wing;
    YsRawPngDecoder Wing2;

    YsSoundPlayer::SoundData Audio1;

    bool isPaused = false;
    bool Question = false;
    std::chrono::steady_clock::time_point startTime;
    double initialVerticalOffset = 0.4;
    double verticalOffset;
    double airfoilAppearTime = 14.0;
    double streamDisapper = 24.0;
    double imageAppearTime = 12.0;

    
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
        verticalOffset=0;
        SetAirfoilPosition(airfoil, initialVerticalOffset);
    }

    saisravy_slide1() : Slide() {
        PrepAudio(Audio1, "saisravy/bernoulli/slide-lift1.wav");
        airfoil = ReadAirfoilData("saisravy/bernoulli/naca.txt.txt");
        if (airfoil.empty()) {
            std::cerr << "Error: Airfoil data could not be loaded." << std::endl;
        }

        if (YSOK != Wing.Decode("saisravy/bernoulli/lift-image2.png")) {
            std::cerr << "Error: Could not load down.png" << std::endl;
        }
        if (YSOK != Wing2.Decode("saisravy/bernoulli/lift-image1.png")) {
            std::cerr << "Error: Could not load down.png" << std::endl;
        }

        if (YSOK != highpressure1.Decode("saisravy/bernoulli/bernoulli-image2.png")) {
            std::cerr << "Error: Could not load down.png" << std::endl;
        }

        if (YSOK != highpressure2.Decode("saisravy/bernoulli/bernoulli-image3.png")) {
            std::cerr << "Error: Could not load down.png" << std::endl;
        }

        if (YSOK != lowpressure1.Decode("saisravy/bernoulli/bernoulli-image4.png")) {
            std::cerr << "Error: Could not load down.png" << std::endl;
        }

        if (YSOK != lowpressure2.Decode("saisravy/bernoulli/bernoulli-image5.png")) {
            std::cerr << "Error: Could not load down.png" << std::endl;
        }

        if (YSOK != arrows_bernoulli.Decode("saisravy/bernoulli/bernoulli-arrows-nobg.png")) {
            std::cerr << "Error: Could not load Bernoulli Arrows from path bernoulli->bernoulli-arrows.png" << std::endl;
        }
        if (YSOK != arrow2.Decode("saisravy/bernoulli/arrow2.png")) {
            std::cerr << "Error: Could not load Bernoulli Arrows from path bernoulli->bernoulli-arrows.png" << std::endl;
        }

        png_flip.Flip();
        SetAirfoilPosition(airfoil, initialVerticalOffset);
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
        //glClearColor(0.565f, 0.933f, 0.565f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawHeading();


        if (elapsedTime < airfoilAppearTime) {
            DrawContent1();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-10.0, 10.0, -5.0, 5.0, 0.1, 20.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslated(-3.0, -2.0, -15.0);
            glRotated(90.0, 0, 1, 0);
       
            DrawCylinder(1.0, 0.7, 2.0, 50, 1.0f, 0.0f, 0.0f);
            glTranslated(0, 0, 2.0);
            DrawCylinder(0.7, 2.0, 2.0, 50, 0.0f, 0.0f, 1.0f);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, 800, 600, 0, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Bernoulli arrows code
            double speed = 300.0;
            double startX = -50.0;
            double endX = 650.0;
            double range = endX - startX;
            int currentX = static_cast<int>(startX + fmod(speed * elapsedTime, range));

            if (currentX < 330) {
                DrawResizedPngImage(arrows_bernoulli, currentX, 355, 1, 1);
            }
            else {
                DrawResizedPngImage(arrow2, currentX, 355, 1, 1);
            }

            int imageIndex = static_cast<int>(elapsedTime) % 2;

            if (imageIndex == 0) {
                DrawResizedPngImage(highpressure1, 150, 475, 0.5, 0.5);
            }
            else {
                DrawResizedPngImage(highpressure2, 150, 475, 0.5, 0.5);
            }

            int imageIndex2 = static_cast<int>(elapsedTime) % 2;
            if (imageIndex2 == 0) {
                DrawResizedPngImage(lowpressure1, 400, 475, 0.5, 0.5);
            }
            else {
                DrawResizedPngImage(lowpressure2, 400, 475, 0.5, 0.5);
            }
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double yOffset = 100.0;

        if (elapsedTime >= airfoilAppearTime) {
            glPushMatrix();
            glTranslated(0, yOffset, 0);
            double verticalOffset;
            if (elapsedTime <= airfoilAppearTime + 18) {
                verticalOffset = DrawAirfoil(airfoil, false);
            }
            else {
                verticalOffset = DrawAirfoil(airfoil, true);
            }
            //DrawAirfoil(airfoil, true);
            glPopMatrix();

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, 800, 600, 0, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            if (elapsedTime <= streamDisapper)
            {
                DisplayImageSequence("saisravy/bernoulli/lift-streamline-frames/", -50, 272 + yOffset, 0.8, 0.8, 11);
            }
            

            
            
            if (elapsedTime > imageAppearTime+10) {
                DrawExtendingArrow(20, 400, 20, 200, 5.0);
            }
            std::vector<std::string> Content2 = {
            "Airfoil shape causes air to flow faster over the top surface creating lower",
             "pressure above the wing.",
             "Higher pressure on the bottom pushes the airfoil upward generating lift."
            };
            DrawTextBox(100, 80, 650, 100, Content2);

            std::vector<std::string> Content3 = {
            "* Formula: P + 1/2pv2 = constant",
            "  (where P is pressure, p is air density, and v is the velocity of the fluid)" };
            if (elapsedTime > imageAppearTime + 20) {
                DrawTextBox(20, 550, 350, 50, Content3);
            }
            

            
            if (elapsedTime > imageAppearTime + 5) {
                std::vector<std::string> highpressure;
                if (elapsedTime < 25){
                    highpressure = {
        "Slow",
        "Airflow", };
                }
 
                else
                {
                    highpressure = {
                        "High",
                        "Pressure",
                    };
                }
                DrawOvalTextBox(600, 520+verticalOffset, 150, 100, highpressure);
            }
            std::vector<std::string> lowpressure = {
    "Low",
    "Pressure",
            };
            if (elapsedTime > imageAppearTime + 7) {
                std::vector<std::string> lowpressure;
                if (elapsedTime < 25) {
                    lowpressure= {
        "Fast",
        "Airflow", };
                }

                else
                {
                    lowpressure = {
                        "Low",
                        "Pressure",
                    };
                }
                DrawOvalTextBox(600, 400+verticalOffset, 150, 100, lowpressure);
            }
            if (elapsedTime > imageAppearTime + 23) {
                DrawResizedPngImage(Wing, 170,200 + yOffset, 1, 1);
            }
            if (elapsedTime > 58) {
                PauseAudio(Audio1);
            }
        }
    }


    std::vector<Point> ReadAirfoilData(const std::string& filename) {
        std::ifstream inputFile(filename);
        std::vector<Point> points;
        double x, y;

        if (!inputFile) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return points;
        }

        while (inputFile >> x >> y) {
            points.push_back({ x, y });
        }
        return points;
    }

    void SetAirfoilPosition(std::vector<Point>& points, double verticalOffset) {
        for (auto& point : points) {
            point.y += verticalOffset;
        }
    }



    void DrawHeading() {
        const char* heading = "BERNOULLI'S PRINCIPLE AND LIFT GENERATION";
        int headingWidth = strlen(heading) * 12;
        int headingX = (800 - headingWidth) / 2;
        int headingY = 50;

        glColor3ub(0, 0, 0);
        for (int offsetX = -1; offsetX <= 1; ++offsetX) {
            for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                glRasterPos2i(headingX + offsetX, headingY + offsetY);
                YsGlDrawFontBitmap12x16(heading);
            }
        }
    }

    void DrawContent1() {
        // textbox dimensions
        int rectX = 50;
        int rectY = 80;
        int rectWidth = 700;
        int rectHeight = 250;

        glColor3ub(230, 230, 250);
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        glColor3ub(0, 0, 0);

        int textX = rectX + 20;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Bernoulli's Principle: States that an increase in the speed of a fluid occurs",
            "  simultaneously with a decrease in pressure.",
            "",
            "* Lift Creation:",
            "  - Airfoil shape causes air to flow faster over the top surface, creating lower",
            "    pressure above the wing.",
            "  - Higher pressure on the bottom pushes the airfoil upward, generating lift.",
            "",
            "* Formula: P + 1/2pv2 = constant",
            "  (where P is pressure, p is air density, and v is the velocity of the fluid)"
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

    void DrawOvalTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
        const int numSegments = 100;
        glColor3ub(255, 255, 0); 
        glBegin(GL_TRIANGLE_FAN);
        glVertex2i(centerX, centerY);  

        for (int i = 0; i <= numSegments; ++i) {
            double angle = 2.0 * M_PI * i / numSegments;
            double x = centerX + (width / 2.0) * cos(angle);
            double y = centerY + (height / 2.0) * sin(angle);
            glVertex2d(x, y);
        }
        glEnd();

        glColor3ub(255, 0, 0);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);

        for (int i = 0; i <= numSegments; ++i) {
            double angle = 2.0 * M_PI * i / numSegments;
            double x = centerX + (width / 2.0) * cos(angle);
            double y = centerY + (height / 2.0) * sin(angle);
            glVertex2d(x, y);
        }
        glEnd();
        glColor3ub(0, 0, 0);  
        int lineHeight = 20;  
        int totalTextHeight = lines.size() * lineHeight;
        int startY = centerY - totalTextHeight / 2 + lineHeight / 2;  

        for (size_t i = 0; i < lines.size(); ++i) {
            int textPosX = centerX - (lines[i].length() * 8) / 2; 
            int textPosY = startY + i * lineHeight;

            glRasterPos2i(textPosX, textPosY);
            YsGlDrawFontBitmap12x16(lines[i].c_str());
        }
    }

    double DrawAirfoil(const std::vector<Point>& points, bool lift) {
        // double verticalOffset = 0.0; 
        const double liftSpeed = 1.0;

        
        if (lift) {
            verticalOffset -= liftSpeed;
            if (verticalOffset < -200) { 
                verticalOffset = -200;
            }
        }
        glColor3ub(200, 200, 200);
        glBegin(GL_POLYGON);
        for (const auto& point : points) {
            int screenX = static_cast<int>(point.x * 700) + 50;
            int screenY = static_cast<int>((1 - point.y) * 500) + 50 + static_cast<int>(verticalOffset);
            glVertex2i(screenX, screenY);
        }
        glEnd();

        glColor3ub(0, 0, 255);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        for (const auto& point : points) {
            int screenX = static_cast<int>(point.x * 700) + 50;
            int screenY = static_cast<int>((1 - point.y) * 500) + 50 + static_cast<int>(verticalOffset);
            glVertex2i(screenX, screenY);
        }
        glEnd();
        return verticalOffset;
    }


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

    void DrawCylinder(double bottomRadius, double topRadius, double height, int numSegments, float r, float g, float b)
    {
        glBegin(GL_QUADS);
        glColor3f(r, g, b);
        for (int i = 0; i < numSegments; ++i)
        {
            double angle1 = 2.0 * M_PI * i / numSegments;
            double angle2 = 2.0 * M_PI * (i + 1) / numSegments;
            double x1Bottom = bottomRadius * cos(angle1);
            double y1Bottom = bottomRadius * sin(angle1);
            double x2Bottom = bottomRadius * cos(angle2);
            double y2Bottom = bottomRadius * sin(angle2);

            double x1Top = topRadius * cos(angle1);
            double y1Top = topRadius * sin(angle1);
            double x2Top = topRadius * cos(angle2);
            double y2Top = topRadius * sin(angle2);

            glVertex3d(x1Bottom, y1Bottom, 0);
            glVertex3d(x2Bottom, y2Bottom, 0);
            glVertex3d(x2Top, y2Top, height);
            glVertex3d(x1Top, y1Top, height);
        }

        glEnd();
    }

    double GetElapsedTime(const std::chrono::steady_clock::time_point& startTime) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        return duration.count() / 1000.0; 
    }
    
    void DisplayImageSequence(const std::string& basePath, int posX, int posY, double scaleX, double scaleY, int totalFrames) {
        YsRawPngDecoder png;
        const double frameDuration = 0.1; 
        double elapsedTime = GetElapsedTime(startTime);
        int currentFrame = static_cast<int>(elapsedTime / frameDuration) % totalFrames;
        char filename[256];
        snprintf(filename, sizeof(filename), "%sframe%02d.png", basePath.c_str(), currentFrame + 1);

        if (YSOK == png.Decode(filename)) {
            png.Flip();
            DrawResizedPngImage(png, posX, posY, scaleX, scaleY);
        }
        else {
            std::cerr << "Error: Could not load " << filename << std::endl;
        }
    }

    
};

class saisravy_slide3_intro : public Slide {
private:
    YsRawPngDecoder png;
    bool isPaused = false;


    double currentArrowLength = 0.0;
    double maxArrowLength = 0.0;
    bool isArrowAnimating = true;
    int posx = 100;
    bool movingRight = true;
    int currentX;
    int currentY;


public:
    void Reset() {
    }

    saisravy_slide3_intro() : Slide() {

        if (YSOK != png.Decode("saisravy/saisravy_slide3/introdrag.png")) {
            std::cerr << "Error: Could not load down.png" << std::endl;
        }
        //png.Flip();
        currentX = 800;
        currentY = 250;
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
        //glClearColor(0.565f, 0.933f, 0.565f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        std::vector<std::string> textlines = {
            "",
            "LEARN MORE ABOUT DRAG",
            "IN THE NEXT SLIDE"
        };

        DrawTextBox(posx, 500, 300, 100, textlines, 800, movingRight);
        DrawResizedPngImage(png, 100, 100, 1, 1);
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

class saisravy_slide2 : public Slide {
private:
    std::vector<Point> airfoil;
    YsSoundPlayer::SoundData Audio1;
    bool isPaused = false;
    bool Question = false;
    std::chrono::steady_clock::time_point startTime;
    double initialVerticalOffset = 0.4;
    double airfoilAppearTime = 14.0;
    double streamDisapper = 24.0;
    double imageAppearTime = 12.0;
    double angle = 0;
    YsRawPngDecoder aoa;
    YsRawPngDecoder meancamberline;


    double currentArrowLength = 0.0;
    double maxArrowLength = 0.0;
    bool isArrowAnimating = true;


public:
    void Reset() {
        startTime = std::chrono::steady_clock::now();
        isPaused = false;
        currentArrowLength = 0.0;
        isArrowAnimating = true;
        initialVerticalOffset = 0.0;
        SetAirfoilPosition(airfoil, initialVerticalOffset);
    }

    saisravy_slide2() : Slide() {
        PrepAudio(Audio1, "saisravy/saisravy_slide2/audio.wav");
        if (YSOK != aoa.Decode("saisravy/saisravy_slide2//aoa.png")) {
            std::cerr << "Error: Could not load aoa.png" << std::endl;
        }

        if (YSOK != meancamberline.Decode("saisravy/saisravy_slide2//mean-camber-line.png")) {
            std::cerr << "Error: Could not load meancamberline.png" << std::endl;
        }


        airfoil = ReadAirfoilData("saisravy/bernoulli/naca.txt.txt");
        if (airfoil.empty()) {
            std::cerr << "Error: Airfoil data could not be loaded." << std::endl;
        }
        SetAirfoilPosition(airfoil, initialVerticalOffset);
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
                return PREVIOUS;
            }
            else if (keyCode == FSKEY_RIGHT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
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
        //glClearColor(0.565f, 0.933f, 0.565f, 1.0f); // green color background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawHeading();
        DrawAirfoil(airfoil, false, angle, 1);
        DrawContent();
        
        if (elapsedTime > 1 && elapsedTime < 37) {
            std::vector<std::string> AirfoilIntroText;
            AirfoilIntroText = { "This is an Airfoil" };
            DrawRectTextBox(150, 320, 250, 50, AirfoilIntroText);
        }
        if (elapsedTime > 3) {
            DrawExtendingArrow(50, 450, 50, 500, 5);
        }
        if (elapsedTime > 5) {
            std::vector<std::string> leadingedge;
            leadingedge = { "Leading edge" };
            DrawTextBox(20, 500, 150, 50, leadingedge);
        }
        if (elapsedTime > 10) {
            DrawExtendingArrow(750, 450, 750, 500, 5);
        }
        if (elapsedTime > 10.1) {
            std::vector<std::string> trailingedge;
            trailingedge = { "Trailing edge" };
            DrawTextBox(620, 500, 150, 50, trailingedge);
        }
        if (elapsedTime > 12) {
            std::vector<std::string> chordline;
            chordline = { "Chord Line" };
            DrawExtendingArrow(250, 450, 250, 500, 5);
            DrawTextBox(200, 500, 150, 50, chordline);
        }
        if (elapsedTime > 37) {
            DrawResizedPngImage(aoa, 30, 250 ,0.5, 0.5);
        }
        if (elapsedTime > 18) {
            std::vector<std::string> uppercamber;
            uppercamber = { "Upper camber" };
            DrawExtendingArrow(450, 400, 450, 350, 5);
            DrawTextBox(400, 300, 120, 50, uppercamber);

            std::vector<std::string> lowercamber;
            lowercamber = { "Lower camber" };
            DrawExtendingArrow(500, 480, 500, 500, 2);
            DrawTextBox(450, 530, 120, 50, lowercamber);
        }



        if (elapsedTime > 49) {
            PauseAudio(Audio1);
        }

        }


    std::vector<Point> ReadAirfoilData(const std::string& filename) {
        std::ifstream inputFile(filename);
        std::vector<Point> points;
        double x, y;

        if (!inputFile) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return points;
        }

        while (inputFile >> x >> y) {
            points.push_back({ x, y });
        }
        return points;
    }

    void SetAirfoilPosition(std::vector<Point>& points, double verticalOffset) {
        for (auto& point : points) {
            point.y += verticalOffset;
        }
    }

    void DrawContent() {
        // Draw a rectangle for text
        int rectX = 50;
        int rectY = 80;
        int rectWidth = 700;
        int rectHeight = 130; // Adjusted height to accommodate the image

        // Rectangle background
        glColor3ub(230, 230, 250); // Lavender color
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        // Rectangle border
        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        // Draw text inside the rectangle
        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 20;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Leading Edge: The front edge of the airfoil.",
            "* Trailing Edge: The rear edge of the airfoil.",
            "* Chord Line: An imaginary straight line from the leading edge to the trailing edge.",
            "* Camber: The curvature of the airfoil's upper and lower surfaces.",
            "* Mean Camber Line: The curve that runs halfway between the top and bottom surfaces.",
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1); // Reset line width
    }
    
    void DrawRectTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
        // Recatngle text box with yellow background to show important message
        glColor3ub(255, 255, 0);
        glBegin(GL_QUADS);
        glVertex2i(centerX - width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY + height / 2);
        glVertex2i(centerX - width / 2, centerY + height / 2);
        glEnd();

        glColor3ub(255, 0, 0);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2i(centerX - width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY + height / 2);
        glVertex2i(centerX - width / 2, centerY + height / 2);
        glEnd();

        glColor3ub(0, 0, 0);
        int lineHeight = 20;
        int totalTextHeight = lines.size() * lineHeight;
        int startY = centerY - totalTextHeight / 2 + lineHeight / 2;

        for (size_t i = 0; i < lines.size(); ++i) {
            int textPosX = centerX - (lines[i].length() * 6);
            int textPosY = startY + i * lineHeight;

            glRasterPos2i(textPosX, textPosY);
            YsGlDrawFontBitmap12x16(lines[i].c_str());
        }
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
    
    double GetElapsedTime(const std::chrono::steady_clock::time_point& startTime) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        return duration.count() / 1000.0;
    }

    void DrawHeading() {
        const char* heading = "AIRFOIL TERMINOLOGY";
        int headingWidth = strlen(heading) * 12;
        int headingX = (800 - headingWidth) / 2;
        int headingY = 50;

        glColor3ub(0, 0, 0);

        // Render the text multiple times with a slight offset for bold effect
        for (int offsetX = -1; offsetX <= 1; ++offsetX) {
            for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                glRasterPos2i(headingX + offsetX, headingY + offsetY);
                YsGlDrawFontBitmap12x16(heading);
            }
        }
    }


    double DrawAirfoil(const std::vector<Point>& points, bool lift, double angle, double scale) {
        static double verticalOffset = 100.0;
        const double liftSpeed = 1.0;

        // Adjust vertical offset for lift
        if (lift) {
            verticalOffset -= liftSpeed;
            if (verticalOffset < -200) {
                verticalOffset = -200;
            }
        }

        // Find the leading and trailing edges
        auto leadingEdge = std::min_element(points.begin(), points.end(),
            [](const Point& a, const Point& b) { return a.x < b.x; });
        auto trailingEdge = std::max_element(points.begin(), points.end(),
            [](const Point& a, const Point& b) { return a.x < b.x; });

        // Compute the center of the airfoil
        double centerX = (leadingEdge->x + trailingEdge->x) / 2.0;
        double centerY = (leadingEdge->y + trailingEdge->y) / 2.0;
        double cosTheta = cos(angle);
        double sinTheta = sin(angle);

        // Draw the airfoil (scaled)
        glColor3ub(200, 200, 200);
        glBegin(GL_POLYGON);
        for (const auto& point : points) {
            // Translation and rotation
            double translatedX = point.x - centerX;
            double translatedY = point.y - centerY;
            double rotatedX = translatedX * cosTheta - translatedY * sinTheta;
            double rotatedY = translatedX * sinTheta + translatedY * cosTheta;

            // Apply scaling
            rotatedX *= scale;
            rotatedY *= scale;

            // Convert to screen coordinates
            int screenX = static_cast<int>((rotatedX + centerX) * 700) + 50;
            int screenY = static_cast<int>((1 - (rotatedY + centerY)) * 500) + 50 + static_cast<int>(verticalOffset);
            glVertex2i(screenX, screenY);
        }
        glEnd();

        // Draw the airfoil outline (scaled)
        glColor3ub(0, 0, 255);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        for (const auto& point : points) {
            double translatedX = point.x - centerX;
            double translatedY = point.y - centerY;

            double rotatedX = translatedX * cosTheta - translatedY * sinTheta;
            double rotatedY = translatedX * sinTheta + translatedY * cosTheta;

            // Apply scaling
            rotatedX *= scale;
            rotatedY *= scale;

            // Convert to screen coordinates
            int screenX = static_cast<int>((rotatedX + centerX) * 700) + 50;
            int screenY = static_cast<int>((1 - (rotatedY + centerY)) * 500) + 50 + static_cast<int>(verticalOffset);
            glVertex2i(screenX, screenY);
        }
        glEnd();

        // Draw line from the leading edge to the trailing edge (scaled)
        glColor3ub(0, 0, 0);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        int leadingEdgeX = static_cast<int>(leadingEdge->x * 700 * scale) + 50;
        int leadingEdgeY = static_cast<int>((1 - leadingEdge->y) * 500 * scale) + 50 + static_cast<int>(verticalOffset);
        int trailingEdgeX = static_cast<int>(trailingEdge->x * 700 * scale) + 50;
        int trailingEdgeY = static_cast<int>((1 - trailingEdge->y) * 500 * scale) + 50 + static_cast<int>(verticalOffset);
        glVertex2i(leadingEdgeX, leadingEdgeY);
        glVertex2i(trailingEdgeX, trailingEdgeY);
        glEnd();

        return verticalOffset;
    }





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
    
};


class saisravy_slide3 : public Slide {
private:
    YsSoundPlayer::SoundData Audio1;
    bool isPaused = false;
    std::chrono::steady_clock::time_point startTime1;
    YsRawPngDecoder aircraft;
    double currentArrowLength = 0.0;
    bool isArrowAnimating = true;
    double maxArrowLength = 0.0;
    bool movement = true;
    int currentX = 800;

    // State for image sequence playback
    struct ImageSequence {
        std::string basePath;
        int totalFrames;
    };

    std::vector<ImageSequence> imageSequences;
    int currentSequenceIndex = 0; // Index of the current sequence
    int frameCounter = 1;         // Current frame number
    double previousTime = 0.0;
    const double frameDuration = 0.1;
    bool isFrozen = false;


public:
    void Reset() {
        movement = true;
        startTime1 = std::chrono::steady_clock::now(); // Reset the start time
        isPaused = false;
        currentArrowLength = 0.0;
        isArrowAnimating = true;
        currentSequenceIndex = 0;
        frameCounter = 1;
        previousTime = GetElapsedTime(startTime1);
        bool isFrozen = false;
    }

    saisravy_slide3() : Slide() {
        PrepAudio(Audio1, "saisravy/saisravy_slide3/audio.wav");
        if (YSOK != aircraft.Decode("saisravy/saisravy_slide3//aircraft.png")) {
            std::cerr << "Error: Could not load aircraft.png" << std::endl;
        }

        // Initialize the image sequences
        imageSequences = {
            {"saisravy/saisravy_slide3/form drag/", 70},
            {"saisravy/saisravy_slide3/skinfricdrag/", 109},
            {"saisravy/saisravy_slide3/induceddrag/", 116}
        };

        Reset(); // Ensure the slide is reset when constructed
    }

    SlideTransition RunSlide() override {
        Reset();
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
                return PREVIOUS;
            }
            else if (keyCode == FSKEY_RIGHT) {
                slideRunning = false;
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Reset();
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
        double elapsedTime = GetElapsedTime(startTime1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawHeading();

        // Check if we should freeze the screen
        if (elapsedTime > 50 && !isFrozen) {
            isFrozen = true;
            PauseAudio(Audio1); // Pause the audio when freezing
        }

        // If the screen is not frozen, update animations
        if (!isFrozen) {
            DrawInitialAnimation(elapsedTime);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            PlaySequentialImageSequences();
        }
        else {
            // If frozen, just render the current frame without updating
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            DisplayFrozenState(elapsedTime);
        }
    }

    void DisplayFrozenState(double elapsedTime) {
        // Render the current frame of the image sequence without updating
        ImageSequence& currentSequence = imageSequences[currentSequenceIndex];
        DisplayImageSequence(currentSequence.basePath, 400, 350, 0.2, 0.2, currentSequence.totalFrames);
        std::vector<std::string> airflow = { "RELATIVE", "AIRFLOW" };
        std::vector<std::string> thrust = { "THRUST" };
        std::vector<std::string> drag = { "DRAG" };

        // Draw airflow and thrust text boxes after 7 seconds
        if (elapsedTime > 5) {
            glColor3ub(255, 0, 0); // Red arrows
            DrawExtendingArrow(180, 140, 250, 140, 1);
            DrawExtendingArrow(330, 180, 270, 180, 1);
        }
        if (elapsedTime > 7) {
            glColor3ub(0, 0, 0); // Black text boxes
            DrawOvalTextBox(180, 80, 180, 100, airflow);
            DrawOvalTextBox(230, 230, 170, 60, thrust);
        }

        // Draw drag arrow and text box after 10 seconds
        if (elapsedTime > 10) {
            glColor3ub(255, 0, 0); // Red arrow
            DrawExtendingArrow(600, 180, 700, 180, 1);
        }
        if (elapsedTime > 11) {
            glColor3ub(0, 0, 0); // Black text box
            DrawOvalTextBox(730, 230, 170, 60, drag);
        }

        // Render the initial animation elements without updates
        DrawResizedPngImageMove(aircraft, 300, 100, 1, 1, false, 800); // Stop movement
        DrawRectTextBox(200, 450, 350, 150, { "Induced Drag::",
            "Resistance due to the generation of lift,",
            "caused by vortices at the wing tips.",
            "",
            "Total Drag:",
            "The sum of parasite drag and induced drag."
            });
    }


    void DrawInitialAnimation(double elapsedTime) {
        // Disable blending and depth testing
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        // Draw informational text box first
        
        if (elapsedTime < 30) {
            std::vector<std::string> parasite_drag = {
            "Skin Friction Drag:",
            "Resistance caused by the surface roughness ",
            "of the object.",
            "",
            "Form Drag:",
            "Resistance due to the shape of the object."
            

            };
            DrawRectTextBox(200, 450, 350, 150, parasite_drag);
        }
        if (elapsedTime >= 30) {
            std::vector<std::string> parasite_drag = {
            "Induced Drag::",
            "Resistance due to the generation of lift,",
            "caused by vortices at the wing tips.",
            "",
            "Total Drag:",
            "The sum of parasite drag and induced drag."
            };
            DrawRectTextBox(200, 450, 350, 150, parasite_drag);
        }
        

        // Draw the moving aircraft PNG image
        DrawResizedPngImageMove(aircraft, 300, 100, 1, 1, movement, 800);

        // Draw arrows after 5 seconds
        if (elapsedTime > 5) {
            glColor3ub(255, 0, 0); // Red arrows
            DrawExtendingArrow(180, 140, 250, 140, 1);
            DrawExtendingArrow(330, 180, 270, 180, 1);
        }

        // Define text labels
        std::vector<std::string> airflow = { "RELATIVE", "AIRFLOW" };
        std::vector<std::string> thrust = { "THRUST" };
        std::vector<std::string> drag = { "DRAG" };

        // Draw airflow and thrust text boxes after 7 seconds
        if (elapsedTime > 7) {
            glColor3ub(0, 0, 0); // Black text boxes
            DrawOvalTextBox(180, 80, 180, 100, airflow);
            DrawOvalTextBox(230, 230, 170, 60, thrust);
        }

        // Draw drag arrow and text box after 10 seconds
        if (elapsedTime > 10) {
            glColor3ub(255, 0, 0); // Red arrow
            DrawExtendingArrow(600, 180, 700, 180, 1);
        }
        if (elapsedTime > 11) {
            glColor3ub(0, 0, 0); // Black text box
            DrawOvalTextBox(730, 230, 170, 60, drag);
        }
    }

    void PlaySequentialImageSequences() {
        double currentTime = GetElapsedTime(startTime1);
        ImageSequence& currentSequence = imageSequences[currentSequenceIndex];

        // Update frame only if enough time has passed
        if (currentTime - previousTime >= frameDuration) {
            previousTime = currentTime;
            frameCounter++;

            // Check if the current sequence is complete
            if (frameCounter > currentSequence.totalFrames) {
                frameCounter = 1; // Reset frame counter
                currentSequenceIndex++; // Move to the next sequence

                // Loop back to the first sequence if we reach the end of the list
                if (currentSequenceIndex >= imageSequences.size()) {
                    currentSequenceIndex = 0;
                }
            }
        }

        // Display the current frame of the active sequence
        DisplayImageSequence(currentSequence.basePath, 400, 350, 0.2, 0.2, currentSequence.totalFrames);
    }

    void DisplayImageSequence(const std::string& basePath, int posX, int posY, double scaleX, double scaleY, int totalFrames) {
        YsRawPngDecoder png;
        char filename[256];
        snprintf(filename, sizeof(filename), "%sframe-%03d.png", basePath.c_str(), frameCounter);

        if (YSOK == png.Decode(filename)) {
            DrawResizedPngImage(png, posX, posY, scaleX, scaleY, false, 800);
        }
        else {
            std::cerr << "Error: Could not load " << filename << std::endl;
        }
    }


    void DrawOvalTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
        const int numSegments = 100;
        glColor3ub(255, 255, 0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2i(centerX, centerY);

        for (int i = 0; i <= numSegments; ++i) {
            double angle = 2.0 * M_PI * i / numSegments;
            double x = centerX + (width / 2.0) * cos(angle);
            double y = centerY + (height / 2.0) * sin(angle);
            glVertex2d(x, y);
        }
        glEnd();

        glColor3ub(255, 0, 0);  // Red border color
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);

        for (int i = 0; i <= numSegments; ++i) {
            double angle = 2.0 * M_PI * i / numSegments;
            double x = centerX + (width / 2.0) * cos(angle);
            double y = centerY + (height / 2.0) * sin(angle);
            glVertex2d(x, y);
        }
        glEnd();
        glColor3ub(0, 0, 0);
        int lineHeight = 20;
        int totalTextHeight = lines.size() * lineHeight;
        int startY = centerY - totalTextHeight / 2 + lineHeight / 2;

        for (size_t i = 0; i < lines.size(); ++i) {
            int textPosX = centerX - (lines[i].length() * 9) / 2;
            int textPosY = startY + i * lineHeight;

            glRasterPos2i(textPosX, textPosY);
            YsGlDrawFontBitmap12x16(lines[i].c_str());
        }
    }
    
    void DrawContent() {
        // Draw a rectangle for text
        int rectX = 50;
        int rectY = 80;
        int rectWidth = 700;
        int rectHeight = 130; // Adjusted height to accommodate the image

        // Rectangle background
        glColor3ub(230, 230, 250); // Lavender color
        glBegin(GL_QUADS);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        // Rectangle border
        glColor3ub(0, 0, 0); // Black border
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(rectX, rectY);
        glVertex2i(rectX + rectWidth, rectY);
        glVertex2i(rectX + rectWidth, rectY + rectHeight);
        glVertex2i(rectX, rectY + rectHeight);
        glEnd();

        // Draw text inside the rectangle
        glColor3ub(0, 0, 0); // Black text color

        int textX = rectX + 20;
        int textY = rectY + 30;
        int lineHeight = 20;

        const char* textLines[] = {
            "* Leading Edge: The front edge of the airfoil.",
            "* Trailing Edge: The rear edge of the airfoil.",
            "* Chord Line: An imaginary straight line from the leading edge to the trailing edge.",
            "* Camber: The curvature of the airfoil's upper and lower surfaces.",
            "* Mean Camber Line: The curve that runs halfway between the top and bottom surfaces.",
        };

        int numLines = sizeof(textLines) / sizeof(textLines[0]);

        for (int i = 0; i < numLines; ++i) {
            glRasterPos2i(textX, textY + i * lineHeight);
            YsGlDrawFontBitmap8x12(textLines[i]);
        }

        glLineWidth(1); // Reset line width
    }

    void DrawRectTextBox(int centerX, int centerY, int width, int height, const std::vector<std::string>& lines) {
        // Disable blending and depth testing
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        // Draw rectangle background
        glColor3ub(255, 255, 0); // Yellow background
        glBegin(GL_QUADS);
        glVertex2i(centerX - width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY + height / 2);
        glVertex2i(centerX - width / 2, centerY + height / 2);
        glEnd();

        // Draw rectangle border
        glColor3ub(255, 0, 0); // Red border
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2i(centerX - width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY - height / 2);
        glVertex2i(centerX + width / 2, centerY + height / 2);
        glVertex2i(centerX - width / 2, centerY + height / 2);
        glEnd();

        // Set text color
        glColor3ub(0, 0, 0); // Black text color

        // Reset matrix and set raster position
        glLoadIdentity();

        int lineHeight = 20;
        int totalTextHeight = lines.size() * lineHeight;
        int startY = centerY - totalTextHeight / 2 + lineHeight / 2;

        // Draw each line of text (left-aligned)
        for (size_t i = 0; i < lines.size(); ++i) {
            // Set text position near the left edge of the text box with some padding
            int textPosX = centerX - width / 2 + 10; // 10-pixel padding from the left edge
            int textPosY = startY + i * lineHeight;

            // Set raster position for text
            glRasterPos2i(textPosX, textPosY);

            // Debug: Print text position
            //std::cout << "Drawing text at (" << textPosX << ", " << textPosY << "): " << lines[i] << std::endl;

            // Draw the text
            YsGlDrawFontBitmap8x12(lines[i].c_str());
        }
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

    double GetElapsedTime(const std::chrono::steady_clock::time_point& startTime) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        return duration.count() / 1000.0;
    }

    void DrawHeading() {
        const char* heading = "DRAG";
        int headingWidth = strlen(heading) * 12;
        int headingX = (800 - headingWidth) / 2;
        int headingY = 50;

        glColor3ub(0, 0, 0);

        // Render the text multiple times with a slight offset for bold effect
        for (int offsetX = -1; offsetX <= 1; ++offsetX) {
            for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                glRasterPos2i(headingX + offsetX, headingY + offsetY);
                YsGlDrawFontBitmap12x16(heading);
            }
        }
    }

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


    void DrawResizedPngImageMove(YsRawPngDecoder& png, int posX, int posY, double scaleX, double scaleY, bool move, int windowWidth) {
        //static int currentX = windowWidth;
        static int currentY = posY;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (move) {
            if (currentX > posX) {
                currentX -= 20;
            }
            else {
                currentX = posX; 
            }
        }
        else {
            currentX = posX;
            currentY = posY;
        }

        for (int y = 0; y < png.hei; ++y) {
            for (int x = 0; x < png.wid; ++x) {
                int screenX = static_cast<int>(currentX + x * scaleX);
                int screenY = static_cast<int>(currentY + y * scaleY);

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
    void DrawResizedPngImage(YsRawPngDecoder& png, int posX, int posY, double scaleX, double scaleY, bool move, int windowWidth) {
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

#endif 
