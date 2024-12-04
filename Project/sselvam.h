#ifndef SSELVAM_H
#define SSELVAM_H

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
#include <math.h>
#include "yspng.h"
#include <vector>
#include <string>
#include <cstring>

#include "slide.h"
/*
This enum declared globally is needed for the main function to run.
It is declared as an enum for the sake of code readability (instead of just using numbers)
DO NOT declare a new enum, append an element onto the end of this one if neccessary
*/





class Slide_sselvam1 : public Slide {
private:
    double x, y, z;
    double scale;
    double speed;
    bool isPaused = false;
    
    // Color definitions
    const GLfloat bodyColor[3] = {1.0f, 1.0f, 1.0f};      // Pure white
    const GLfloat windowColor[3] = {0.2f, 0.2f, 0.2f};    // Dark gray
    const GLfloat accentColor[3] = {1.0f, 0.6f, 0.0f};    // Orange
    const GLfloat wingColor[3] = {1.0f, 1.0f, 1.0f};      // Pure white
    const GLfloat wheelColor[3] = {0.2f, 0.2f, 0.2f};     // Dark gray
    const GLfloat runwayColor[3] = {0.7f, 0.7f, 0.7f};    // Gray
    
    YsSoundPlayer::SoundData audio;

    void DrawTitle() {
        glColor3f(0.1f, 0.1f, 0.3f);  
        glRasterPos3f(-8.0f, 10.0f, -30.0f);  
        const char* title = "FLY 2 SURVIVE";
        YsGlDrawFontBitmap28x44(title);
        
        glRasterPos3f(-6.0f, 8.0f, -30.0f);  
        const char* tagline = "- BIT BY BIT";
        YsGlDrawFontBitmap16x20(tagline);
    }

    void DrawRunway() {
        glColor3fv(runwayColor);
        glBegin(GL_QUADS);
        glVertex3f(-12.0f, -6.0f, -400.0f);
        glVertex3f(12.0f, -6.0f, -400.0f);
        glVertex3f(12.0f, -6.0f, 40.0f);
        glVertex3f(-12.0f, -6.0f, 40.0f);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        for(float z = -390; z < 40; z += 10) {
            glBegin(GL_QUADS);
            glVertex3f(-0.5f, -5.99f, z);
            glVertex3f(0.5f, -5.99f, z);
            glVertex3f(0.5f, -5.99f, z + 5);
            glVertex3f(-0.5f, -5.99f, z + 5);
            glEnd();
        }
    }

    void DrawBuilding(float x, float y, float z, float width, float height, float depth) {
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_QUADS);
        glVertex3f(x-width, y, z);
        glVertex3f(x+width, y, z);
        glVertex3f(x+width, y+height, z);
        glVertex3f(x-width, y+height, z);
        glEnd();

        glColor3f(0.9f, 0.9f, 0.0f);
        float windowSize = 0.3f;
        float windowSpacing = 0.8f;
        
        for(float h = y + 1.0f; h < y + height - 1.0f; h += windowSpacing) {
            for(float w = x - width + 0.5f; w < x + width - 0.5f; w += windowSpacing) {
                glBegin(GL_QUADS);
                glVertex3f(w, h, z+0.1f);
                glVertex3f(w+windowSize, h, z+0.1f);
                glVertex3f(w+windowSize, h+windowSize, z+0.1f);
                glVertex3f(w, h+windowSize, z+0.1f);
                glEnd();
            }
        }
    }

    void DrawTree(float x, float y, float z, float r, float g, float b) {
        glColor3f(0.5f, 0.35f, 0.05f);
        glBegin(GL_QUADS);
        glVertex3f(x-0.3f, y, z);
        glVertex3f(x+0.3f, y, z);
        glVertex3f(x+0.2f, y+2.0f, z);
        glVertex3f(x-0.2f, y+2.0f, z);
        glEnd();

        glColor3f(r, g, b);
        glBegin(GL_TRIANGLES);
        glVertex3f(x-1.5f, y+1.5f, z);
        glVertex3f(x+1.5f, y+1.5f, z);
        glVertex3f(x, y+4.0f, z);
        
        glVertex3f(x-1.2f, y+2.5f, z);
        glVertex3f(x+1.2f, y+2.5f, z);
        glVertex3f(x, y+5.0f, z);
        glEnd();
    }


    void DrawBackground() {
        glColor3f(0.2f, 0.8f, 0.2f);  
        glBegin(GL_QUADS);
        // Left side ground
        glVertex3f(-50.0f, -6.0f, -400.0f);
        glVertex3f(-12.0f, -6.0f, -400.0f);
        glVertex3f(-12.0f, -6.0f, 40.0f);
        glVertex3f(-50.0f, -6.0f, 40.0f);
        
        // Right side ground
        glVertex3f(12.0f, -6.0f, -400.0f);
        glVertex3f(50.0f, -6.0f, -400.0f);
        glVertex3f(50.0f, -6.0f, 40.0f);
        glVertex3f(12.0f, -6.0f, 40.0f);
        glEnd();

        // Buildings on the sides
        for(float z = -300.0f; z < 20.0f; z += 30.0f) {
            DrawBuilding(-20.0f, -6.0f, z, 3.0f, 15.0f, 5.0f);
            DrawBuilding(20.0f, -6.0f, z, 3.0f, 15.0f, 5.0f);
        }

        // Trees with varying green colors
        for(float z = -350.0f; z < 30.0f; z += 20.0f) {
            DrawTree(-14.0f, -6.0f, z, 0.0f, 0.6f, 0.0f);
            DrawTree(-16.0f, -6.0f, z+10.0f, 0.0f, 0.5f, 0.0f);
            DrawTree(14.0f, -6.0f, z, 0.0f, 0.7f, 0.0f);
            DrawTree(16.0f, -6.0f, z+10.0f, 0.0f, 0.55f, 0.0f);
        }
    }
    
    
    void DrawTurbine(float x, float y, float z) {
    GLUquadricObj *quadric = gluNewQuadric();
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Support strut connecting to wing
    glColor3f(0.7f, 0.7f, 0.7f);  
    glBegin(GL_QUADS);
    glVertex3f(-0.2f, 0.0f, -0.2f);
    glVertex3f(0.2f, 0.0f, -0.2f);
    glVertex3f(0.2f, 1.0f, -0.2f);
    glVertex3f(-0.2f, 1.0f, -0.2f);
    
    glVertex3f(-0.2f, 0.0f, 0.2f);
    glVertex3f(0.2f, 0.0f, 0.2f);
    glVertex3f(0.2f, 1.0f, 0.2f);
    glVertex3f(-0.2f, 1.0f, 0.2f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.94f);
    glRotatef(90, 0, 0, 1);

    gluCylinder(quadric, 0.7, 0.6, 1.8, 32, 1);

    glColor3f(0.3f, 0.3f, 0.3f);
    gluDisk(quadric, 0.65, 0.7, 32, 1);
    
    // Front intake
    float radiusStep = 0.65f / 5.0f;
    for(int i = 0; i < 5; i++) {
        float shade = 0.1f + (0.1f * i);  
        glColor3f(shade, shade, shade);
        gluDisk(quadric, radiusStep * i, radiusStep * (i + 1), 32, 1);
    }

    // Fan blades 
     glColor3f(1.0f, 1.0f, 0.94f);
    for(int i = 0; i < 8; i++) {
        glRotatef(45, 0, 0, 1);  
        glBegin(GL_TRIANGLES);
        glVertex3f(0, 0, 0.1f);
        glVertex3f(0.3f, 0.1f, 0.1f);
        glVertex3f(0.3f, -0.1f, 0.1f);
        glEnd();
    }

    // Back exhaust
    glTranslatef(0, 0, 1.8);
    glColor3f(0.2f, 0.2f, 0.2f);  
    gluDisk(quadric, 0.0, 0.6, 32, 1);
    
    // Exhaust 
    glColor3f(0.3f, 0.3f, 0.3f);
    gluDisk(quadric, 0.55, 0.6, 32, 1);
    glColor3f(0.15f, 0.15f, 0.15f);
    gluDisk(quadric, 0.0, 0.2, 32, 1);
    
    gluDeleteQuadric(quadric);
    glPopMatrix();
  }


    void DrawWheel(float x, float y, float z, float radius) {
        glPushMatrix();
        glTranslatef(x, y, z);
        
        // Wheel strut
        glBegin(GL_QUADS);
        glVertex3f(-0.1f, 0.0f, 0.1f);
        glVertex3f(0.1f, 0.0f, 0.1f);
        glVertex3f(0.1f, 0.8f, 0.1f);
        glVertex3f(-0.1f, 0.8f, 0.1f);
        
        glVertex3f(-0.1f, 0.0f, -0.1f);
        glVertex3f(0.1f, 0.0f, -0.1f);
        glVertex3f(0.1f, 0.8f, -0.1f);
        glVertex3f(-0.1f, 0.8f, -0.1f);
        glEnd();

        GLUquadricObj *quadric = gluNewQuadric();
        glRotatef(90, 1, 0, 0);
        
        gluDisk(quadric, 0, radius, 16, 1);
        gluCylinder(quadric, radius, radius, 0.15, 16, 1);
        glTranslatef(0, 0, 0.15);
        gluDisk(quadric, 0, radius, 16, 1);
        
        gluDeleteQuadric(quadric);
        glPopMatrix();
    }

    void DrawLandingGear() {
        glColor3fv(wheelColor);
        DrawWheel(0.0f, -2.0f, 0.3f, 0.15f);
        DrawWheel(-2.0f, -2.0f, 0.0f, 0.15f);
        DrawWheel(2.0f, -2.0f, 0.0f, 0.15f);
    }

    void DrawAirplane() {
        glPushMatrix();
        glTranslated(x, y, z);
        glScaled(scale, scale, scale);

        // Main body
        glColor3fv(bodyColor);
        GLUquadricObj *quadric = gluNewQuadric();
        
        glPushMatrix();
        gluDisk(quadric, 0, 2.0, 32, 1);
        glTranslatef(0, 0, 1);
        gluCylinder(quadric, 2.0, 1.5, 1.0, 32, 1);
        glPopMatrix();

        // Cockpit windows
        glColor3fv(windowColor);
        glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.2f);
        glVertex3f(1.0f, 1.0f, 0.2f);
        glVertex3f(1.0f, 0.5f, 0.2f);
        glVertex3f(-1.0f, 0.5f, 0.2f);
        glEnd();


        // Left wing
        glColor3fv(wingColor);
        glBegin(GL_POLYGON);
        glVertex3f(-8.0f, -0.5f, 0.0f);
        glVertex3f(-8.0f, 0.5f, 0.0f);
        glVertex3f(-2.0f, 0.5f, -0.5f);
        glVertex3f(-2.0f, -0.5f, -0.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glVertex3f(-8.0f, -0.5f, 0.0f);
        glVertex3f(-8.0f, 0.5f, 0.0f);
        glVertex3f(-2.0f, 0.5f, 0.5f);
        glVertex3f(-2.0f, -0.5f, 0.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glVertex3f(-8.0f, 0.5f, 0.0f);
        glVertex3f(-2.0f, 0.5f, -0.5f);
        glVertex3f(-2.0f, 0.5f, 0.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glVertex3f(-8.0f, -0.5f, 0.0f);
        glVertex3f(-2.0f, -0.5f, -0.5f);
        glVertex3f(-2.0f, -0.5f, 0.5f);
        glEnd();
        DrawTurbine(-5.0f, -1.0f, -1.2f); 

        // Right wing
        glColor3fv(wingColor);
        glBegin(GL_POLYGON);
        glVertex3f(8.0f, -0.5f, 0.0f);
        glVertex3f(8.0f, 0.5f, 0.0f);
        glVertex3f(2.0f, 0.5f, -0.5f);
        glVertex3f(2.0f, -0.5f, -0.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glVertex3f(8.0f, -0.5f, 0.0f);
        glVertex3f(8.0f, 0.5f, 0.0f);
        glVertex3f(2.0f, 0.5f, 0.5f);
        glVertex3f(2.0f, -0.5f, 0.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glVertex3f(8.0f, 0.5f, 0.0f);
        glVertex3f(2.0f, 0.5f, -0.5f);
        glVertex3f(2.0f, 0.5f, 0.5f);
        glEnd();

        glBegin(GL_POLYGON);
        glVertex3f(8.0f, -0.5f, 0.0f);
        glVertex3f(2.0f, -0.5f, -0.5f);
        glVertex3f(2.0f, -0.5f, 0.5f);
        glEnd();
        DrawTurbine(5.0f, -1.0f, -1.2f);

        // Vertical Rudder
        glColor3fv(wingColor);
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 3.0f, -0.2f);
        glVertex3f(-0.6f, 1.0f, -0.2f);
        glVertex3f(0.6f, 1.0f, -0.2f);

        glVertex3f(0.0f, 3.0f, 0.2f);
        glVertex3f(-0.6f, 1.0f, 0.2f);
        glVertex3f(0.6f, 1.0f, 0.2f);
        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(-0.6f, 1.0f, -0.2f);
        glVertex3f(0.0f, 3.0f, -0.2f);
        glVertex3f(0.0f, 3.0f, 0.2f);
        glVertex3f(-0.6f, 1.0f, 0.2f);

        glVertex3f(0.6f, 1.0f, -0.2f);
        glVertex3f(0.0f, 3.0f, -0.2f);
        glVertex3f(0.0f, 3.0f, 0.2f);
        glVertex3f(0.6f, 1.0f, 0.2f);
        glEnd();

        // Orange stripe
        glColor3fv(accentColor);
        glBegin(GL_QUADS);
        glVertex3f(-1.8f, -0.2f, 0.1f);
        glVertex3f(1.8f, -0.2f, 0.1f);
        glVertex3f(1.8f, -0.8f, 0.1f);
        glVertex3f(-1.8f, -0.8f, 0.1f);
        glEnd();

        DrawLandingGear();
        gluDeleteQuadric(quadric);
        glPopMatrix();
    }

    void Update() {
        z += speed;
        scale = 1.0 + (z + 60) / 50.0;
        
        if(z > 15 || !IsAudioPlaying(audio)) {  
        z = -60;
        scale = 1.0;
        if(!IsAudioPlaying(audio)) {  
            PlayBGMAudio(audio);
        }
     }
  }

    void Draw() {
        DrawBackground();
        DrawRunway();
        DrawAirplane();
        DrawTitle();
    }

public:
    Slide_sselvam1() : Slide() {
        x = 0;
        y = -2.0;
        z = -60;
        scale = 1.0;
        speed = 0.1;
    }

    virtual SlideTransition RunSlide() override {
        PrepAudio(audio, "sselvam/Aeroplane_intro.wav"); 
        PlayBGMAudio(audio);
        
        glEnable(GL_DEPTH_TEST); 

        while(FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int key = FsInkey();
            
            if(key == FSKEY_LEFT) {
                StopAll(audio);  
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return PREVIOUS;
            } 
            else if(key == FSKEY_RIGHT) {
                StopAll(audio);  
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return NEXT;
            }
            else if(key == FSKEY_ESC) {
                StopAll(audio);  
                return STAY;
            }
            else if(key == FSKEY_SPACE) {
                isPaused = !isPaused;
                if(isPaused) {
                    PauseAudio(audio);
                } else {
                    ResumeAudio(audio);
                }
            } else if (key == FSKEY_ESC) {
                return EXIT;
            }

            player.KeepPlaying(); 

            if(!isPaused) {
                Update();
            }
            glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, 800, 600);
            
            glMatrixMode(GL_PROJECTION);  
            glLoadIdentity();
            gluPerspective(35.0, 800.0/600.0, 0.1, 100.0);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            gluLookAt(0.0, 12.0, 25.0,
                     0.0, -2.0, -20.0,
                     0.0, 1.0, 0.0);
            
    
            
            Draw();
            
            FsSwapBuffers();
            FsSleep(25);
        }
        return STAY;
    }
};


class Slide_sselvam2 : public Slide {
private:
    struct Box {
        std::string title;
        std::vector<std::string> content;
        double x, y, width, height;
        double opacity;
        double targetX;
        bool isActive;
        bool isExiting;
        double displayDuration;
    };

    struct ImageWithCaption {
        YsRawPngDecoder png;
        std::string caption;
        int x, y;

        ImageWithCaption() : x(0), y(0) {}
        
        ImageWithCaption(const ImageWithCaption& other) : 
            caption(other.caption), x(other.x), y(other.y) {
            if(other.png.rgba != nullptr) {
                png.wid = other.png.wid;
                png.hei = other.png.hei;
                png.rgba = new unsigned char[other.png.wid * other.png.hei * 4];
                memcpy(png.rgba, other.png.rgba, other.png.wid * other.png.hei * 4);
            }
        }

        ImageWithCaption& operator=(const ImageWithCaption& other) {
            if(this != &other) {
                caption = other.caption;
                x = other.x;
                y = other.y;
                if(other.png.rgba != nullptr) {
                    delete[] png.rgba;
                    png.wid = other.png.wid;
                    png.hei = other.png.hei;
                    png.rgba = new unsigned char[other.png.wid * other.png.hei * 4];
                    memcpy(png.rgba, other.png.rgba, other.png.wid * other.png.hei * 4);
                }
            }
            return *this;
        }

        ~ImageWithCaption() {
        }
    };

    std::vector<Box> boxes;
    std::vector<ImageWithCaption> images;
    int currentBox = 0;
    double timeCounter = 0;
    double moveSpeed = 10.0;
    bool centerReached = false;
    bool isPaused = false;
    YsSoundPlayer::SoundData audio;
    const int windowWidth = 800;
    const int windowHeight = 600;

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

        double yOffset = 70;
        for(const auto& line : box.content) {
            const int lineLen = strlen(line.c_str()) * 12;
            double lineX = box.x + (box.width - lineLen) / 2;
            glColor4f(0, 0, 0, box.opacity);
            DrawBoldText(line.c_str(), lineX, box.y + yOffset, false);
            yOffset += 25;
        }

        glDisable(GL_BLEND);
    }

    void DrawTitle(const char* title) {
        const int titleLen = strlen(title) * 16;
        DrawBoldText(title, (windowWidth - titleLen) / 2, 50, true);
    }

    void DrawScaledImage(const YsRawPngDecoder& png, int x, int y, int targetWidth, int targetHeight) {
        float scaleX = (float)targetWidth / png.wid;
        float scaleY = (float)targetHeight / png.hei;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;
        
        int finalWidth = png.wid * scale;
        int finalHeight = png.hei * scale;
        int offsetX = (targetWidth - finalWidth) / 2;
        
        glRasterPos2i(x + offsetX, y);
        glPixelZoom(scale, scale);
        glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
        glPixelZoom(1.0f, 1.0f);
    }

    void DrawImages() {
        const int imgWidth = 220;
        const int imgHeight = 160;
        
        for(const auto& img : images) {
            DrawScaledImage(img.png, img.x, img.y, imgWidth, imgHeight);

            const int captionLen = strlen(img.caption.c_str()) * 8;
            double captionX = img.x + (imgWidth - captionLen) / 2;
            glRasterPos2d(captionX, img.y + imgHeight - 140);
            YsGlDrawFontBitmap8x12(img.caption.c_str());
        }
    }

    void UpdateAnimation() {
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
        } else if(!box.isExiting && timeCounter >= box.displayDuration && box.displayDuration > 0) {
            box.isExiting = true;
            timeCounter = 0;
        } else if(box.isExiting) {
            if(currentBox < boxes.size() - 1) {
                box.x += moveSpeed;
                if(box.x > windowWidth + 100) {
                    currentBox++;
                    centerReached = false;
                    if(currentBox < boxes.size()) {
                        boxes[currentBox].isActive = true;
                    }
                }
            }
        }
    }
}

    void InitializeBoxes() {
    boxes = {
        {
            "Types of Wings/Wingspan",
            {
                "Passenger Aircraft (60-80 meters)",
                "- Optimized for long-distance flights",
                "- Providing fuel efficiency and stability",
                "",
                "Jet Fighter (10-15 meters)",
                "- Short wings for high-speed agility",
                "- Suited for quick maneuvers over shorter ranges",
                "",
                "Gladiator (6-10 meters)",
                "- Compact wings offering strong lift at low speeds",
                "- Ideal for close-range and early aviation missions"
            },
            -500, 75, 600, 350, 1.0,
            static_cast<double>((windowWidth - 500) / 2),
            true,
            false,
            61.0  
        },
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
            -500, 90, 600, 250, 1.0,
            static_cast<double>((windowWidth - 500) / 2),
            false,
            false,
            19.0  
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
            -500, 90, 600, 250, 1.0,
            static_cast<double>((windowWidth - 500) / 2),
            false,
            false,
            30  
        },
        {
            "Takeaway Message",
            {
                "Wing control surfaces—like flaps and",
                "ailerons manage lift, drag, and",
                "stability, ensuring safe and efficient",
                "flight maneuverability."
            },
            -500, 90, 500, 200, 1.0,
            static_cast<double>((windowWidth - 500) / 2),
            false,
            false,
            -1.0  
        }
     };
   }
    void LoadImages() {
        images.resize(3);
        
        if(YSOK != images[0].png.Decode("sselvam/slide3.png") ||
           YSOK != images[1].png.Decode("sselvam/takeoff.png") ||
           YSOK != images[2].png.Decode("sselvam/landing.png")) {
            std::cout << "Failed to load PNG images" << std::endl;
            return;
        }
        
        for(auto& img : images) {
            img.png.Flip();
        }

        const int imgWidth = 220;
        const int imgHeight = 160;
        const int imgSpacing = 30;
        const int totalWidth = imgWidth * 3 + imgSpacing * 2;
        int startX = (windowWidth - totalWidth) / 2;
        int imgY = 550;

        images[0].caption = "wing control surfaces";
        images[1].caption = "Wings while takeoff";
        images[2].caption = "Wings while landing";

        for(int i = 0; i < 3; ++i) {
            images[i].x = startX + i * (imgWidth + imgSpacing);
            images[i].y = imgY;
        }
    }

    void Render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        DrawTitle("Wing Control Surfaces and Their Function");

        for(auto& box : boxes) {
            if(box.isActive) {
                DrawBox(box);
            }
        }

        DrawImages();

        FsSwapBuffers();
    }

public:
    Slide_sselvam2() : Slide() {
        InitializeBoxes();
        LoadImages();
    }

    SlideTransition RunSlide() override {
    PrepAudio(audio, "sselvam/slide3.wav");
    PlayBGMAudio(audio);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(FsCheckWindowOpen() && slideRunning) {
        FsPollDevice();
        int key = FsInkey();  

        if(key == FSKEY_ESC) {
            StopAll(audio);
            return STAY;
        }
        else if(key == FSKEY_LEFT) {
            StopAll(audio);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return PREVIOUS;
        }
        else if(key == FSKEY_RIGHT) {
            StopAll(audio);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return NEXT;
        }
        else if(key == FSKEY_SPACE) {
            isPaused = !isPaused;
            if(isPaused) {
                PauseAudio(audio);  
            } else {
                ResumeAudio(audio);  
            }
        } else if (key == FSKEY_ESC) {
                return EXIT;
            }

        player.KeepPlaying();  

        glViewport(0, 0, windowWidth, windowHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if(!isPaused) {
            timeCounter += 0.025;
            UpdateAnimation();
        }

        Render();
        FsSleep(20);
    }
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    return STAY;
    }
};

class Slide_sselvam3 : public Slide {
private:
    const int windowWidth = 800;
    const int windowHeight = 600;
    bool isPaused = false;
    YsSoundPlayer::SoundData audio;

    struct Label {
        std::string text;
        double startX, startY;    
        double targetX, targetY;  
        double currentX, currentY;
        bool isAnimating;
        double speed;
        bool useSmallFont;
    };

    YsRawPngDecoder png1;  
    YsRawPngDecoder png2; 
    std::vector<Label> labels;
    double timeElapsed;
    const double delayBetweenLabels = 0.5;

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

    void InitializeLabels() {
    labels = {
        {"Winglet\n(Decrease Drag)", 
            -200, 100,
            550, 150,
            -200, 100,
            false, 1.0,  
            false},
        {"Wing\n(Generate Lift)", 
            -200, 200,
            420, 230,
            -200, 200,
            false, 1.0,
            false},
        {"Leading Edge Slats\n(Increase Lift)", 
            -200, 300,
            245, 325,
            -200, 300,
            false, 1.0,
            false},
        {"Aileron\n(Change Roll)", 
            static_cast<double>(windowWidth + 200), 150,
            620, 375,
            static_cast<double>(windowWidth + 200), 150,
            false, 1.0,
            false},
        {"Spoiler\n(Air/Speed breaks)", 
            static_cast<double>(windowWidth + 200), 250,
            550, 435,
            static_cast<double>(windowWidth + 200), 250,
            false, 1.0,
            false},
        {"Trailing Edge Flaps\n(Increase Lift and Drag)", 
            static_cast<double>(windowWidth + 200), 400,
            500, 565,
            static_cast<double>(windowWidth + 200), 400,
            false, 1.0,
            false},
        {"Vertical Stabilizer\n(Control Yaw)", 
            static_cast<double>(windowWidth + 200), 90,
            245, 30,
            static_cast<double>(windowWidth + 200), 90,
            false, 1.0,
            false},
        {"Rudder\n(Change Yaw)", 
            static_cast<double>(windowWidth + 200), 140,
            325, 70,
            static_cast<double>(windowWidth + 200), 140,
            false, 1.0,
            false},
        {"Horizontal Stabilizer\n(Control Pitch)", 
            -200, 220,
            5, 285,
            -200, 220,
            false, 1.0,
            false},    
        {"Elevator\n(Change Pitch)", 
            -200, 200,
            15, 230,
            -200, 200,
            false, 1.0,
            false},
       };
    }
public:
    Slide_sselvam3() : Slide() {
        // Use absolute path or correct relative path to PNG files
        std::string png1Path = "sselvam/wings.png";
        std::string png2Path = "sselvam/wingsssss.png";

        if(YSOK != png1.Decode(png1Path.c_str())) {
            printf("Failed to load %s\n", png1Path.c_str());
        }
        if(png1.wid > 0) {
            png1.Flip();
        }

        if(YSOK != png2.Decode(png2Path.c_str())) {
            printf("Failed to load %s\n", png2Path.c_str());
        }
        if(png2.wid > 0) {
            png2.Flip();
        }

        InitializeLabels();
        timeElapsed = 0;
    }

    virtual SlideTransition RunSlide() override {
    PrepAudio(audio, "sselvam/slide2.wav");
    PlayBGMAudio(audio);
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    const double initialDelay = 6.0; 
    const double delayBetweenLabels = 5.2;  

    while(FsCheckWindowOpen() && slideRunning) {
        FsPollDevice();
        int key = FsInkey();
        
        if(key == FSKEY_LEFT) {
            StopAll(audio);
            return PREVIOUS;
        }
        else if(key == FSKEY_RIGHT) {
            StopAll(audio);
            return NEXT;
        }
        else if(key == FSKEY_ESC) {
            StopAll(audio);
            return STAY;
        }
        else if(key == FSKEY_SPACE) {
            isPaused = !isPaused;
            if(isPaused) {
                PauseAudio(audio);
            } else {
                ResumeAudio(audio);
            }
        } else if (key == FSKEY_ESC) {
                return EXIT;
            }

        player.KeepPlaying();

        if(!isPaused) {
            timeElapsed += 0.025;

            for(size_t i = 0; i < labels.size(); ++i) {
                if(timeElapsed >= initialDelay + (i * delayBetweenLabels) && !labels[i].isAnimating) {
                    labels[i].isAnimating = true;
                }
            }

            // Update positions for active animations
            for(Label& label : labels) {
                if(label.isAnimating) {
                    double dx = label.targetX - label.currentX;
                    double dy = label.targetY - label.currentY;
                    double distance = sqrt(dx*dx + dy*dy);
                    
                    if(distance > 1.0) {
                        label.currentX += (dx * label.speed * 0.05);
                        label.currentY += (dy * label.speed * 0.05);
                    }
                }
            }
        }

        glViewport(0, 0, windowWidth, windowHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        if(png1.wid > 0) {
            int imageX = (windowWidth - png1.wid) / 2 + 100;
            int imageY = (windowHeight - png1.hei) / 2 + 40;
            float scale1 = 0.86f;
            glRasterPos2i(imageX, imageY + png1.hei);
            glPixelZoom(scale1, scale1);
            glDrawPixels(png1.wid, png1.hei, GL_RGBA, GL_UNSIGNED_BYTE, png1.rgba);
        }

        if(png2.wid > 0) {
            float scale2 = 0.55f;
            glRasterPos2i(10, windowHeight - 325);
            glPixelZoom(scale2, scale2);
            glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);
        }

        glPixelZoom(1.0f, 1.0f);

        glColor3ub(0, 0, 0);
        for(const Label& label : labels) {
            if(label.isAnimating) {
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
        }

        FsSwapBuffers();
        FsSleep(20);
    }
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    return STAY;
    }
};





#endif
