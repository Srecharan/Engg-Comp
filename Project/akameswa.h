#ifndef AKAMESWA_H
#define AKAMESWA_H

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

class Slide_akameswa : public Slide {
private:
    const int windowWidth = 800;
    const int windowHeight = 600;
    const int tableStartY = 150;
    const int rowHeight = 40;
    const int colWidth = 200;
    
    YsSoundPlayer::SoundData audio;
    YsSoundPlayer::SoundData subsonicWav;
    YsSoundPlayer::SoundData supersonicWav;
    YsSoundPlayer::SoundData narrationWav;
    bool isNarrationPlaying = true;
    YsRawPngDecoder subsonicImage;
    YsRawPngDecoder supersonicImage;
    bool isPaused = false;

    // Animation variables
    double animationProgress = 0.0;
    const double animationSpeed = 0.005;
    bool animationComplete = false;
    double titleBounceProgress = 0.0;
    std::vector<double> cellFlipProgress;
    const int numRows = 5;
    const int numCols = 3;

    std::vector<std::string> characteristics = {
        "Inlet Design",
        "Operating Speed",
        "Thrust Priority",
        "Engine Type",
        "Application"
    };

    std::vector<std::string> subsonic = {
        "Thick, rounded lip",
        "Below Mach 0.8",
        "Fuel efficiency",
        "High-bypass turbofan",
        "A-10 Thunderbolt II"
    };

    std::vector<std::string> supersonic = {
        "Sharp, pointed lip",
        "Above Mach 1.0",
        "Maximum thrust",
        "Low-bypass with afterburner",
        "NASA X-59"
    };

public:
    Slide_akameswa() : Slide() {
        // Initialize images
        subsonicImage.Decode("akameswa/subsonic.png");
        supersonicImage.Decode("akameswa/supersonic.png");
        subsonicImage.Flip();
        supersonicImage.Flip();
        
        // Load all audio files using PrepAudio instead of direct LoadWav
        PrepAudio(narrationWav, "akameswa/narration.wav");
        PrepAudio(subsonicWav, "akameswa/subsonic_engine.wav");
        PrepAudio(supersonicWav, "akameswa/supersonic_engine.wav");
        
        PlayBGMAudio(narrationWav);
        
        // Initialize flip animations
        cellFlipProgress.resize(numRows * numCols, 0.0);
    }

private:
    double EaseOutElastic(double t) {
        const double c4 = (2.0 * M_PI) / 3.0;
        return t == 0.0 ? 0.0 : t == 1.0 ? 1.0 : 
               pow(2.0, -10.0 * t) * sin((t * 10.0 - 0.75) * c4) + 1.0;
    }

    double EaseOutCubic(double t) {
        return 1.0 - pow(1.0 - t, 3);
    }

    bool areAllCellsComplete() {
        for(const double& progress : cellFlipProgress) {
            if(progress < 1.0) return false;
        }
        return true;
    }

public:
    SlideTransition RunSlide() {
        // Remove initial PlayBGMAudio since we now do it in constructor
        while (FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int keyCode = FsInkey();
            // Remove mouse state variables since we're not using them anymore

            if (!isPaused && !animationComplete) {
                // Update all animations
                titleBounceProgress += animationSpeed;
                if(titleBounceProgress > 1.0) titleBounceProgress = 1.0;

                animationProgress += animationSpeed;
                if(animationProgress > 1.0) animationProgress = 1.0;

                // Update cell flips with cascade
                for (int i = 0; i < cellFlipProgress.size(); i++) {
                    if (animationProgress > 0.3 + (i * 0.1)) {  // Delayed start
                        cellFlipProgress[i] += animationSpeed * 2.0;
                        if(cellFlipProgress[i] > 1.0) cellFlipProgress[i] = 1.0;
                    }
                }

                // Check completion
                if(titleBounceProgress >= 1.0 && 
                   animationProgress >= 1.0 && 
                   areAllCellsComplete()) {
                    animationComplete = true;
                }
            }

            if (keyCode == FSKEY_SPACE) {
                if (!isPaused) {
                    StopAudio(narrationWav);
                    StopAudio(subsonicWav);
                    StopAudio(supersonicWav);
                    isPaused = true;
                } else {
                    if (!IsAudioPlaying(narrationWav)) {
                        PlayBGMAudio(narrationWav);
                    }
                    isPaused = false;
                }
            }

            if (keyCode == FSKEY_1) {
                printf("Playing subsonic sound\n");
                StopAudio(narrationWav);
                StopAudio(supersonicWav);
                PlayBGMAudio(subsonicWav);
            }
            if (keyCode == FSKEY_2) {
                printf("Playing supersonic sound\n");
                StopAudio(narrationWav);
                StopAudio(subsonicWav);
                PlayBGMAudio(supersonicWav);
            }

            player.KeepPlaying();
            Render();
            FsSleep(20);

            if (keyCode == FSKEY_LEFT) {
                StopAudio(narrationWav);
                StopAudio(subsonicWav);
                StopAudio(supersonicWav);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return PREVIOUS;
            } else if (keyCode == FSKEY_RIGHT) {
                StopAudio(narrationWav);
                StopAudio(subsonicWav);
                StopAudio(supersonicWav);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return NEXT;
            } else if (keyCode == FSKEY_ESC) {
                return EXIT;
            }
        }
        return STAY;
    }

private:
    void DrawTable() {
        const int tableWidth = colWidth * 3;
        const int tableStartX = (windowWidth - tableWidth) / 2;

        glColor3ub(0, 0, 0);
        
        // Draw static table grid
        for(int i = 0; i <= characteristics.size(); i++) {
            glBegin(GL_LINES);
            glVertex2i(tableStartX, tableStartY + i * rowHeight);
            glVertex2i(tableStartX + tableWidth, tableStartY + i * rowHeight);
            glEnd();
        }

        for(int i = 0; i <= 3; i++) {
            glBegin(GL_LINES);
            glVertex2i(tableStartX + i * colWidth, tableStartY);
            glVertex2i(tableStartX + i * colWidth, tableStartY + characteristics.size() * rowHeight);
            glEnd();
        }

        double easedProgress = EaseOutCubic(animationProgress);
        
        // Draw column headers with animation
        double col1Y = tableStartY - 20 + (windowHeight - (tableStartY - 20)) * (1.0 - easedProgress);
        glRasterPos2i(tableStartX + 20, (int)col1Y);
        YsGlDrawFontBitmap12x16("Characteristic");
        
        double col2Y = tableStartY - 20 - (tableStartY - 20) * (1.0 - easedProgress);
        glRasterPos2i(tableStartX + colWidth + 20, (int)col2Y);
        YsGlDrawFontBitmap12x16("Subsonic");
        
        double col3Y = tableStartY - 20 + (windowHeight - (tableStartY - 20)) * (1.0 - easedProgress);
        glRasterPos2i(tableStartX + colWidth * 2 + 20, (int)col3Y);
        YsGlDrawFontBitmap12x16("Supersonic");

        // Draw table content with combined animations
        for(int i = 0; i < characteristics.size(); i++) {
            int cellIndex;
            double rowX, rowY, flip, scale;
            
            // First column
            cellIndex = i * numCols;
            rowY = tableStartY + i * rowHeight + 25;
            if(i % 2 == 0) { // 1st, 3rd, 5th rows from right
                rowX = tableStartX + 10 + (windowWidth - (tableStartX + 10)) * (1.0 - easedProgress);
            } else { // 2nd and 4th rows from left
                rowX = tableStartX + 10 - (tableStartX + 10) * (1.0 - easedProgress);
            }
            
            // Add flip animation
            flip = cellFlipProgress[cellIndex] * M_PI;
            scale = cos(flip);
            if(scale < 0) scale = -scale;
            
            glPushMatrix();
            glTranslated(rowX, rowY, 0);
            glScaled(scale, 1.0, 1.0);
            glColor3ub(0, 0, 0);
            glRasterPos2i(0, 0);
            YsGlDrawFontBitmap8x12(characteristics[i].c_str());
            glPopMatrix();
            
            // Middle column from top
            cellIndex = i * numCols + 1;
            rowY = tableStartY + i * rowHeight + 25 - tableStartY * (1.0 - easedProgress);
            flip = cellFlipProgress[cellIndex] * M_PI;
            scale = cos(flip);
            if(scale < 0) scale = -scale;
            
            glPushMatrix();
            glTranslated(tableStartX + colWidth + 10, rowY, 0);
            glScaled(scale, 1.0, 1.0);
            glRasterPos2i(0, 0);
            YsGlDrawFontBitmap8x12(subsonic[i].c_str());
            glPopMatrix();
            
            // Last column from bottom
            cellIndex = i * numCols + 2;
            rowY = tableStartY + i * rowHeight + 25 + (windowHeight - (tableStartY + i * rowHeight + 25)) * (1.0 - easedProgress);
            flip = cellFlipProgress[cellIndex] * M_PI;
            scale = cos(flip);
            if(scale < 0) scale = -scale;
            
            glPushMatrix();
            glTranslated(tableStartX + colWidth * 2 + 10, rowY, 0);
            glScaled(scale, 1.0, 1.0);
            glRasterPos2i(0, 0);
            YsGlDrawFontBitmap8x12(supersonic[i].c_str());
            glPopMatrix();
        }
    }

    void DrawImages() {
        const int imageWidth = 200;
        const int imageHeight = 150;
        const int spacing = 50;
        const int totalWidth = imageWidth * 2 + spacing;
        const int startX = (windowWidth - totalWidth) / 2;
        const int imageY = tableStartY + characteristics.size() * rowHeight * 1.6 + 50;

        if(animationProgress > 0.8) {
            glRasterPos2i(startX, imageY);
            glDrawPixels(subsonicImage.wid, subsonicImage.hei, GL_RGBA, GL_UNSIGNED_BYTE, subsonicImage.rgba);

            glRasterPos2i(startX + imageWidth + spacing, imageY);
            glDrawPixels(supersonicImage.wid, supersonicImage.hei, GL_RGBA, GL_UNSIGNED_BYTE, supersonicImage.rgba);
        }
    }

    void Render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw bouncing title
        const char* heading = "Aircraft Engine Types: Subsonic vs Supersonic";
        int headingWidth = strlen(heading) * 12;
        int headingX = (windowWidth - headingWidth) / 2;
        
        double titleY = 50.0;
        if(titleBounceProgress > 0.0) {
            double bounce = EaseOutElastic(titleBounceProgress);
            titleY = 50.0 - (1.0 - bounce) * 100.0;
        }
        
        glColor3ub(0, 0, 0);
        glRasterPos2i(headingX, (int)titleY);
        YsGlDrawFontBitmap12x16(heading);

        DrawTable();
        DrawImages();
        
        // Draw instructions
        glColor3ub(0, 0, 0);
        glRasterPos2i(50, windowHeight - 50);
        YsGlDrawFontBitmap8x12("Press 1: Play Subsonic Sound  |  Press 2: Play Supersonic Sound  |  Space: Pause/Resume");
        
        FsSwapBuffers();
    }
};

#endif
