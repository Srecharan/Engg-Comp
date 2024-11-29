

#ifndef ACAR_H
#define ACAR_H

#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "yspng.cpp"
#include "ysglfontdata.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>

#include "slide.h"

class Slide_acar1 : public Slide {
private:
    double aerofoilAngle = 0.0;
    bool dragging = false;
    YsSoundPlayer::SoundData audio, audiocheer, audiowarn;
    bool isPaused = false;

    YsRawPngDecoder cockpitPng, thin, thick, symmetric, glider, commercial, jet, takeoff, landing, supersonic, subsonic;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point currentTime;
    std::chrono::duration<float> elapsedTime;

    int lb, mb, rb, mx, my;
    int keyCode = FsInkey();

    int stage = 5;
    float offset;
    int horizonY = 200;
    float timeloop = 0.2;

    int buttonX1 = 400-150;
    int button1Y = 300;
    int button2Y = 400;
    int button3Y = 500;
    int buttonWidth = 300;
    int buttonHeight = 60;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int choice4 = 0;

    std::vector<std::tuple<int, int, int, int>> goodchoices = {{1, 3, 1, 1}, {2, 2, 1, 1}, {3, 1, 1, 2}};
    bool success;
    std::tuple<int, int, int, int> chosen;


public:
    Slide_acar1() : Slide() {}

    SlideTransition RunSlide() 
    {
        PrepAudio(audio, "acar/crowd.wav"); // If you have more than 1 audio file, load them into audio1, audio2, etc. You'll have to call the PrepAudio fn for each of them
        PrepAudio(audiocheer, "acar/cheer.wav");
        PrepAudio(audiowarn, "acar/warning.wav");
        // PlayBGMAudio(audio);
        LoadAssets();
        startTime = std::chrono::high_resolution_clock::now();
        while (FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            keyCode = FsInkey();
            FsGetMouseState(lb, mb, rb, mx, my);

            if (keyCode == FSKEY_SPACE) {
                if (!isPaused) {
                    StopAudio(audio);  // Pause audio
                    isPaused = true;
                } else {
                    PlayBGMAudio(audio);  // Resume audio
                    isPaused = false;
                }
            }
            player.KeepPlaying();
            // ActiveAnimation(mx, my, lb, mb, rb); // This is outside of the below loop because I want to be able to interact (active animation) even if Paused

            if(!isPaused)
            {
                
            }
            currentTime = std::chrono::high_resolution_clock::now();
            elapsedTime = currentTime - startTime;
            Render();

            FsSleep(20);
            
            /*
            This section below takes care of global slide controls (PREV, NEXT, etc.).
            The return types are of type SlideTransition, which is a globally defined enum (declared at the very top of this code)
            If any further controls are required, modify the enum accordingly by adding your keyword to the end
            */ 
        
            if (keyCode == FSKEY_LEFT) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return PREVIOUS;
            } else if (keyCode == FSKEY_RIGHT) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return NEXT;
            } else if (keyCode == FSKEY_ESC) {
                return EXIT;
            }

        }
        return STAY;
    }


private:
/*
You can add any additional functions here, whatever is needed to make the slide.
Private Functions don't get inherited
*/
    void Render() {

        switch(stage) {
        case 1:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // DrawAerofoil();
            DrawBackground(false);
            // DrawCockpit();
            // drawFlashingWarning();
            drawblur();
            Question1();
            
            FsSwapBuffers();
            break;

        case 2:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // DrawAerofoil();
            DrawBackground(false);
            // DrawCockpit();
            // drawFlashingWarning();
            drawblur();
            Question2();
            
            FsSwapBuffers();
            break;
        
        case 3:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // DrawAerofoil();
            DrawBackground(false);
            // DrawCockpit();
            // drawFlashingWarning();
            drawblur();
            Question3();
            
            FsSwapBuffers();
            break;
        
        case 4:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // DrawAerofoil();
            DrawBackground(false);
            // DrawCockpit();
            // drawFlashingWarning();
            drawblur();
            Question4();
            
            FsSwapBuffers();
            break;

        case 5:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // DrawAerofoil();
            DrawBackground(false);
            // DrawCockpit();
            // drawFlashingWarning();
            TitleSlide();
            
            FsSwapBuffers();
            break;

        case 7:
            chosen = {choice1, choice2, choice3, choice4};
            success = std::find(goodchoices.begin(), goodchoices.end(), chosen) != goodchoices.end();
            if(success==true){
                stage=10;
                PlayForever(audiocheer);
            }
            else{
                stage=11;
                PlayForever(audiowarn);
            }
            break;

        case 10:
            // DrawAerofoil();
            DrawBackground(true);
            DrawCockpit();
            drawSuccess();
            PlayAgain();
            FsSwapBuffers();
            
            break;

        case 11:
            // DrawAerofoil();
            DrawBackground(true);
            DrawCockpit();
            drawFlashingWarning();
            PlayAgain();
            FsSwapBuffers();
            
            break;
        

        
        }
    }

    void LoadAssets(){

        if (YSOK != cockpitPng.Decode("acar/cockpit.png"))
        {
            printf("Failed to load cockpit.png\n");
            exit(1);
        }
        cockpitPng.Flip();

        if (YSOK != thin.Decode("acar/thin.png"))
        {
            printf("Failed to load thin.png\n");
            exit(1);
        }
        thin.Flip();

        if (YSOK != thick.Decode("acar/thick.png"))
        {
            printf("Failed to load thick.png\n");
            exit(1);
        }
        thick.Flip();

        if (YSOK != symmetric.Decode("acar/symmetric.png"))
        {
            printf("Failed to load symmetric.png\n");
            exit(1);
        }
        symmetric.Flip();

        if (YSOK != glider.Decode("acar/glider.png"))
        {
            printf("Failed to load glider.png\n");
            exit(1);
        }
        glider.Flip();

        if (YSOK != jet.Decode("acar/jet.png"))
        {
            printf("Failed to load jet.png\n");
            exit(1);
        }
        jet.Flip();

        if (YSOK != commercial.Decode("acar/commercial.png"))
        {
            printf("Failed to load commercial.png\n");
            exit(1);
        }
        commercial.Flip();

        if (YSOK != takeoff.Decode("acar/Takeoff.png"))
        {
            printf("Failed to load takeoff.png\n");
            exit(1);
        }
        takeoff.Flip();

        if (YSOK != landing.Decode("acar/Landing.png"))
        {
            printf("Failed to load landing.png\n");
            exit(1);
        }
        landing.Flip();

        if (YSOK != subsonic.Decode("acar/subsonic.png"))
        {
            printf("Failed to load subsonic.png\n");
            exit(1);
        }
        subsonic.Flip();

        if (YSOK != supersonic.Decode("acar/supersonic.png"))
        {
            printf("Failed to load supersonic.png\n");
            exit(1);
        }
        supersonic.Flip();
    }
    
    void DrawCockpit(){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(0, cockpitPng.hei - 1); 
        glDrawPixels(cockpitPng.wid, cockpitPng.hei, GL_RGBA, GL_UNSIGNED_BYTE, cockpitPng.rgba);
        glDisable(GL_BLEND);
    }

    void drawFlashingWarning() {
        float alpha = 0.25f * (1.0f + sin(10*elapsedTime.count()));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.0f, 0.0f, alpha);   
        glBegin(GL_QUADS);
            glVertex2f(0, 0); 
            glVertex2f( 0, 600); 
            glVertex2f( 800,  600); 
            glVertex2f(800,  0); 
        glEnd();
        glDisable(GL_BLEND);
        glColor3f(0.2, 0, 0); 
        glRasterPos2f(400 - 100, 130 ); 
        char* devtext = "Failure...";
        YsGlDrawFontBitmap32x48(devtext);
        glRasterPos2f(400 - 220, 160 ); 
        devtext = "Your parts did not have synergy";
        YsGlDrawFontBitmap16x24(devtext);
    }

    void drawSuccess() {
        float alpha = 0.25f * (1.0f + sin(1*elapsedTime.count()));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 1.0f, 0.0f, alpha);   
        glBegin(GL_QUADS);
            glVertex2f(0, 0); 
            glVertex2f( 0, 600); 
            glVertex2f( 800,  600); 
            glVertex2f(800,  0); 
        glEnd();
        glDisable(GL_BLEND);
        glColor3f(0, 0.2, 0); 
        glRasterPos2f(400 - 120, 180 ); 
        char* devtext = "Success!!!";
        YsGlDrawFontBitmap32x48(devtext);
        
    }

    void PlayAgain()
    {
        glColor3ub(50, 50, 50); 
        glBegin(GL_QUADS);
        glVertex2i(250, 300); 
        glVertex2i(250, 400); 
        glVertex2i(550, 400); 
        glVertex2i(550, 300); 
        glEnd();
        glColor3f(1.0, 1.0, 1.0); 
        glRasterPos2f(400 - 100, 300 + 50 ); 
        const char* devtext = "Play Again?";
        YsGlDrawFontBitmap20x28(devtext);

        float alpha = 0.5f * (1.0f + sin(3*elapsedTime.count())); 

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1.0f, 1.0f, 1.0f, alpha); 
        glRasterPos2f(400 - 70, 300 + 80 );
        devtext = "(Press Enter)";
        YsGlDrawFontBitmap12x16(devtext);

        glDisable(GL_BLEND);
        if (keyCode==FSKEY_ENTER) {
            std::cout << "Button 1 clicked!" << std::endl;
            resetslide();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void resetslide(){
        choice1 = 0;
        choice2 = 0;
        choice3 = 0;
        choice4 = 0;
        stage = 1;
        StopAudio(audiocheer);
        StopAudio(audiowarn);
    }

    void drawblur() {
        float alpha = 0.8f;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);   
        glBegin(GL_QUADS);
            glVertex2f(0, 0); 
            glVertex2f( 0, 600); 
            glVertex2f( 800,  600); 
            glVertex2f(800,  0); 
        glEnd();
        glDisable(GL_BLEND);
    }

    void DrawRoad() {
        glColor3ub(50, 50, 50); 

        glBegin(GL_QUADS);
        glVertex2i(200, 600); 
        glVertex2i(600, 600); 
        glVertex2i(450, horizonY); 
        glVertex2i(350, horizonY); 
        glEnd();
    }

    void DrawGrass() {
        glColor3ub(0, 255, 0); 

        glBegin(GL_QUADS);
        glVertex2i(0, horizonY);
        glVertex2i(800, horizonY);
        glVertex2i(800, 600);
        glVertex2i(0, 600);
        glEnd();

    }

    void DrawSky() {
        glColor3ub(135, 206, 235); 

        glBegin(GL_QUADS);
        glVertex2i(0, horizonY);   
        glVertex2i(800, horizonY); 
        glVertex2i(800, 0);
        glVertex2i(0, 0);  
        glEnd();
    }

    void DrawRoadLines() {
        int lineHeight = 30;
        int lineWidth = 10;
        offset = (100.0/timeloop)*(fmod(elapsedTime.count(), timeloop)-timeloop/2.0);
            int yPos = offset + horizonY;

            if (yPos > horizonY && yPos < 600) { 
                float scaleFactor = (float)(yPos - 300) / (600 - 300);

                int centerX = 400;

                glColor3ub(255, 255, 255); 
                glBegin(GL_QUADS);
                glVertex2i(centerX + lineWidth/2, yPos - lineHeight/2);
                glVertex2i(centerX - lineWidth/2, yPos - lineHeight/2);
                glVertex2i(centerX - lineWidth/2, yPos + lineHeight/2);
                glVertex2i(centerX + lineWidth/2, yPos + lineHeight/2);
                glEnd();
            }
        // }
    }

    void DrawBackground(bool drawlines=true) {
        DrawGrass();
        DrawRoad();
        if (drawlines==true){
            DrawRoadLines();
        }
        
        DrawSky();
    }

    void Question1(){
        glColor3ub(0, 0, 0); 
        glRasterPos2f(400 - 300, 100 ); 
        const char* devtext = "Choose your Airfoil";
        YsGlDrawFontBitmap32x48(devtext);

        float alpha = 0.5f * (1.0f + sin(3*elapsedTime.count())); 

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, alpha); 
        glRasterPos2f(400 - 150, 100 + 50); 
        devtext = "(Press 1, 2 or 3)";
        YsGlDrawFontBitmap20x28(devtext);

        glDisable(GL_BLEND);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(400-150, button1Y); 
        glDrawPixels(thin.wid, thin.hei, GL_RGBA, GL_UNSIGNED_BYTE, thin.rgba);

        glRasterPos2i(400-150, button2Y); 
        glDrawPixels(thick.wid, thick.hei, GL_RGBA, GL_UNSIGNED_BYTE, thick.rgba);

        glRasterPos2i(400-150, button3Y); 
        glDrawPixels(symmetric.wid, symmetric.hei, GL_RGBA, GL_UNSIGNED_BYTE, symmetric.rgba);
        glDisable(GL_BLEND);

        if (keyCode==FSKEY_1) {
            std::cout << "Button 1 clicked!" << std::endl;
            stage=2;
            choice1 = 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        else if (keyCode==FSKEY_2) {
            std::cout << "Button 2 clicked!" << std::endl;
            stage=2;
            choice1 = 2;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        else if (keyCode==FSKEY_3) {
            std::cout << "Button 3 clicked!" << std::endl;
            stage=2;
            choice1 = 3;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void Question2(){
        glColor3ub(0, 0, 0); 
        glRasterPos2f(400 - 310, 100 ); 
        const char* devtext = "Choose your wingspan";
        YsGlDrawFontBitmap32x48(devtext);

        float alpha = 0.5f * (1.0f + sin(3*elapsedTime.count())); 

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, alpha); 
        glRasterPos2f(400 - 150, 100 + 50); 
        devtext = "(Press 1, 2 or 3)";
        YsGlDrawFontBitmap20x28(devtext);

        glDisable(GL_BLEND);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(400-150, button1Y); 
        glDrawPixels(jet.wid, jet.hei, GL_RGBA, GL_UNSIGNED_BYTE, jet.rgba);

        glRasterPos2i(400-150, button2Y); 
        glDrawPixels(commercial.wid, commercial.hei, GL_RGBA, GL_UNSIGNED_BYTE, commercial.rgba);

        glRasterPos2i(400-150, button3Y); 
        glDrawPixels(glider.wid, glider.hei, GL_RGBA, GL_UNSIGNED_BYTE, glider.rgba);
        glDisable(GL_BLEND);

        if (keyCode==FSKEY_1) {
            std::cout << "Button 1 clicked!" << std::endl;
            stage=3;
            choice2 = 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        else if (keyCode==FSKEY_2) {
            std::cout << "Button 2 clicked!" << std::endl;
            stage=3;
            choice2 = 2;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        else if (keyCode==FSKEY_3) {
            std::cout << "Button 3 clicked!" << std::endl;
            stage=3;
            choice2 = 3;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void Question3(){
        glColor3ub(0, 0, 0); 
        glRasterPos2f(400 - 360, 100 ); 
        const char* devtext = "Choose flap position";
        YsGlDrawFontBitmap32x48(devtext);

        float alpha = 0.5f * (1.0f + sin(3*elapsedTime.count())); 

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, alpha); 
        glRasterPos2f(400 - 120, 100 + 50); 
        devtext = "(Press 1 or 2)";
        YsGlDrawFontBitmap20x28(devtext);

        glDisable(GL_BLEND);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(400-150, button1Y); 
        glDrawPixels(takeoff.wid, takeoff.hei, GL_RGBA, GL_UNSIGNED_BYTE, takeoff.rgba);

        glRasterPos2i(400-150, button3Y); 
        glDrawPixels(landing.wid, landing.hei, GL_RGBA, GL_UNSIGNED_BYTE, landing.rgba);
        glDisable(GL_BLEND);

        if (keyCode==FSKEY_1) {
            std::cout << "Button 1 clicked!" << std::endl;
            stage=4;
            choice3 = 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        else if (keyCode==FSKEY_2) {
            std::cout << "Button 2 clicked!" << std::endl;
            stage=4;
            choice3 = 2;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }


    void Question4(){
        glColor3ub(0, 0, 0); 
        glRasterPos2f(400 - 310, 100 ); 
        const char* devtext = "Choose your Engine";
        YsGlDrawFontBitmap32x48(devtext);

        float alpha = 0.5f * (1.0f + sin(3*elapsedTime.count())); 

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, alpha); 
        glRasterPos2f(400 - 120, 100 + 50); 
        devtext = "(Press 1 or 2)";
        YsGlDrawFontBitmap20x28(devtext);

        glDisable(GL_BLEND);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(400-80, button1Y+50); 
        glDrawPixels(subsonic.wid, subsonic.hei, GL_RGBA, GL_UNSIGNED_BYTE, subsonic.rgba);


        glRasterPos2i(400-110, button3Y+50); 
        glDrawPixels(supersonic.wid, supersonic.hei, GL_RGBA, GL_UNSIGNED_BYTE, supersonic.rgba);
        glDisable(GL_BLEND);

        if (keyCode==FSKEY_1) {
            std::cout << "Button 1 clicked!" << std::endl;
            stage=7;
            choice4 = 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        else if (keyCode==FSKEY_2) {
            std::cout << "Button 2 clicked!" << std::endl;
            stage=7;
            choice4 = 2;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void TitleSlide() {
        glColor3ub(0, 0, 0); 
        glRasterPos2f(400 - 200, 100); 
        const char* devtext = "Fly 2 Survive";
        YsGlDrawFontBitmap32x48(devtext);

        glRasterPos2f(400 - 120, 100 + 70); 
        devtext = "The Game";
        YsGlDrawFontBitmap24x40(devtext);

        glColor3ub(255, 255, 255);
        glRasterPos2f(400 - 100, 100 + 140); 
        devtext = "Your Mission";
        YsGlDrawFontBitmap16x20(devtext);

        glRasterPos2f(400 - 300, 100 + 170); 
        devtext = "Create an aircraft capable of taking off";
        YsGlDrawFontBitmap16x20(devtext);

        float alpha = 0.5f * (1.0f + sin(3*elapsedTime.count())); 

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1.0f, 1.0f, 1.0f, alpha); 
        glRasterPos2f(400 - 100, 100 + 290); 
        devtext = "(Press Enter)";
        YsGlDrawFontBitmap20x28(devtext);

        glDisable(GL_BLEND);

        if (keyCode==FSKEY_ENTER){
            stage = 1;
        }
    }
};



#endif
