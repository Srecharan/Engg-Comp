//fp.cpp
#include <stdio.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <math.h>

class AirplaneAnimation {
private:
    double x, y, z;
    double scale;
    double speed;
    
    const GLfloat bodyColor[3] = {1.0f, 1.0f, 1.0f};      // Pure white
    const GLfloat windowColor[3] = {0.2f, 0.2f, 0.2f};    // Dark gray
    const GLfloat accentColor[3] = {1.0f, 0.6f, 0.0f};    // Orange
    const GLfloat wingColor[3] = {1.0f, 1.0f, 1.0f};      // Pure white
    const GLfloat wheelColor[3] = {0.2f, 0.2f, 0.2f};     // Dark gray
    const GLfloat runwayColor[3] = {0.7f, 0.7f, 0.7f};    // Gray

    void DrawTitle() {
    glColor3f(0.1f, 0.1f, 0.3f);  // Dark blue color for text
    glRasterPos3f(-8.0f, 10.0f, -30.0f);  // Adjusted position
    const char* title = "FLY 2 SURVIVE";
    YsGlDrawFontBitmap28x44(title);  // Using built-in font
    
    glRasterPos3f(-6.0f, 8.0f, -30.0f);  // Position for tagline
    const char* tagline = "- BIT BY BIT";
    YsGlDrawFontBitmap16x20(tagline);
    }

    void DrawRunway() {
        glColor3fv(runwayColor);
        glBegin(GL_QUADS);
        // Extended runway much further back and made it wider
        glVertex3f(-12.0f, -6.0f, -400.0f);  // Much further back (-400 instead of -200)
        glVertex3f(12.0f, -6.0f, -400.0f);
        glVertex3f(12.0f, -6.0f, 40.0f);    // Extended forward too
        glVertex3f(-12.0f, -6.0f, 40.0f);
        glEnd();

        // Center lines extended to match
        glColor3f(1.0f, 1.0f, 1.0f);
        for(float z = -390; z < 40; z += 10) {  // Adjusted to match new runway length
            glBegin(GL_QUADS);
            glVertex3f(-0.5f, -5.99f, z);
            glVertex3f(0.5f, -5.99f, z);
            glVertex3f(0.5f, -5.99f, z + 5);
            glVertex3f(-0.5f, -5.99f, z + 5);
            glEnd();
        }
    } 

public:
    AirplaneAnimation() {
        x = 0;
        y = -2.0;  // Start lower
        z = -60;   // Start further back
        scale = 1.0;
        speed = 0.1;  // Slightly slower approach
    }
     
    void DrawBuilding(float x, float y, float z, float width, float height, float depth) {
        // Building body
        glColor3f(0.7f, 0.7f, 0.7f);  // Light gray for building
        glBegin(GL_QUADS);
        // Front
        glVertex3f(x-width, y, z);
        glVertex3f(x+width, y, z);
        glVertex3f(x+width, y+height, z);
        glVertex3f(x-width, y+height, z);
        glEnd();

        // Windows
        glColor3f(0.9f, 0.9f, 0.0f);  // Yellow for lit windows
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
    // Tree trunk
    glColor3f(0.5f, 0.35f, 0.05f);  // Brown
    glBegin(GL_QUADS);
    glVertex3f(x-0.3f, y, z);
    glVertex3f(x+0.3f, y, z);
    glVertex3f(x+0.2f, y+2.0f, z);
    glVertex3f(x-0.2f, y+2.0f, z);
    glEnd();

    // Tree top with passed color
    glColor3f(r, g, b);  // Custom green shade
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
    glColor3f(0.2f, 0.8f, 0.2f);  // Brighter green for base ground
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
        // Left side buildings
        DrawBuilding(-20.0f, -6.0f, z, 3.0f, 15.0f, 5.0f);
        // Right side buildings
        DrawBuilding(20.0f, -6.0f, z, 3.0f, 15.0f, 5.0f);
    }

    // Trees between buildings and runway
    for(float z = -350.0f; z < 30.0f; z += 20.0f) {
        // Left side trees with varying green colors
        DrawTree(-14.0f, -6.0f, z, 0.0f, 0.6f, 0.0f);  // Medium green
        DrawTree(-16.0f, -6.0f, z+10.0f, 0.0f, 0.5f, 0.0f);  // Slightly darker
        // Right side trees
        DrawTree(14.0f, -6.0f, z, 0.0f, 0.7f, 0.0f);  // Lighter green
        DrawTree(16.0f, -6.0f, z+10.0f, 0.0f, 0.55f, 0.0f);  // Another shade
    }
} 
   
    void DrawAirplane() {
        glPushMatrix();
        glTranslated(x, y, z);
        glScaled(scale, scale, scale);

        // Main body (circular front)
        glColor3fv(bodyColor);
        GLUquadricObj *quadric = gluNewQuadric();
        
        // Front circular part
        glPushMatrix();
        gluDisk(quadric, 0, 2.0, 32, 1);
        
        // Nose cone
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

        // Wings - Increased wingspan
        // Wings - Made thicker with vertical height
        glColor3fv(wingColor);
        // Left wing - thicker with vertical dimension
        glBegin(GL_POLYGON);
        // Front face of wing
        glVertex3f(-8.0f, -0.5f, 0.0f);   // Bottom wing tip
        glVertex3f(-8.0f, 0.5f, 0.0f);    // Top wing tip
        glVertex3f(-2.0f, 0.5f, -0.5f);   // Top body joint front
        glVertex3f(-2.0f, -0.5f, -0.5f);  // Bottom body joint front
        glEnd();

        glBegin(GL_POLYGON);
        // Back face of wing
        glVertex3f(-8.0f, -0.5f, 0.0f);   // Bottom wing tip
        glVertex3f(-8.0f, 0.5f, 0.0f);    // Top wing tip
        glVertex3f(-2.0f, 0.5f, 0.5f);    // Top body joint back
        glVertex3f(-2.0f, -0.5f, 0.5f);   // Bottom body joint back
        glEnd();

        // Top and bottom surfaces
        glBegin(GL_POLYGON);
        // Top surface
        glVertex3f(-8.0f, 0.5f, 0.0f);    // Wing tip
        glVertex3f(-2.0f, 0.5f, -0.5f);   // Body joint front
        glVertex3f(-2.0f, 0.5f, 0.5f);    // Body joint back
        glEnd();

        glBegin(GL_POLYGON);
        // Bottom surface
        glVertex3f(-8.0f, -0.5f, 0.0f);   // Wing tip
        glVertex3f(-2.0f, -0.5f, -0.5f);  // Body joint front
        glVertex3f(-2.0f, -0.5f, 0.5f);   // Body joint back
        glEnd();

        // Right wing - mirror of left wing
        glBegin(GL_POLYGON);
        // Front face of wing
        glVertex3f(8.0f, -0.5f, 0.0f);    // Bottom wing tip
        glVertex3f(8.0f, 0.5f, 0.0f);     // Top wing tip
        glVertex3f(2.0f, 0.5f, -0.5f);    // Top body joint front
        glVertex3f(2.0f, -0.5f, -0.5f);   // Bottom body joint front
        glEnd();

        glBegin(GL_POLYGON);
        // Back face of wing
        glVertex3f(8.0f, -0.5f, 0.0f);    // Bottom wing tip
        glVertex3f(8.0f, 0.5f, 0.0f);     // Top wing tip
        glVertex3f(2.0f, 0.5f, 0.5f);     // Top body joint back
        glVertex3f(2.0f, -0.5f, 0.5f);    // Bottom body joint back
        glEnd();

        // Top and bottom surfaces
        glBegin(GL_POLYGON);
        // Top surface
        glVertex3f(8.0f, 0.5f, 0.0f);     // Wing tip
        glVertex3f(2.0f, 0.5f, -0.5f);    // Body joint front
        glVertex3f(2.0f, 0.5f, 0.5f);     // Body joint back
        glEnd();

        glBegin(GL_POLYGON);
        // Bottom surface
        glVertex3f(8.0f, -0.5f, 0.0f);    // Wing tip
        glVertex3f(2.0f, -0.5f, -0.5f);   // Body joint front
        glVertex3f(2.0f, -0.5f, 0.5f);    // Body joint back
        glEnd();

        // Vertical Rudder on top (visible from front view)
        glColor3fv(wingColor);
        // Main rudder body
        glBegin(GL_TRIANGLES);
        // Front face
        glVertex3f(0.0f, 3.0f, -0.2f);     // Top point
        glVertex3f(-0.6f, 1.0f, -0.2f);    // Base left
        glVertex3f(0.6f, 1.0f, -0.2f);     // Base right

        // Back face
        glVertex3f(0.0f, 3.0f, 0.2f);      // Top point
        glVertex3f(-0.6f, 1.0f, 0.2f);     // Base left
        glVertex3f(0.6f, 1.0f, 0.2f);      // Base right
        glEnd();

        // Connect front and back faces
        glBegin(GL_QUADS);
        // Left side
        glVertex3f(-0.6f, 1.0f, -0.2f);
        glVertex3f(0.0f, 3.0f, -0.2f);
        glVertex3f(0.0f, 3.0f, 0.2f);
        glVertex3f(-0.6f, 1.0f, 0.2f);

        // Right side
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

        // Landing gear
        DrawLandingGear();

        gluDeleteQuadric(quadric);
        glPopMatrix();
    }

    void DrawWheel(float x, float y, float z, float radius) {
        glPushMatrix();
        glTranslatef(x, y, z);
        
        // Improved wheel strut (thicker and shorter)
        glBegin(GL_QUADS);  // Changed from lines to quads for thicker strut
        // Front face of strut
        glVertex3f(-0.1f, 0.0f, 0.1f);    // Bottom front left
        glVertex3f(0.1f, 0.0f, 0.1f);     // Bottom front right
        glVertex3f(0.1f, 0.8f, 0.1f);     // Top front right
        glVertex3f(-0.1f, 0.8f, 0.1f);    // Top front left
        
        // Back face of strut
        glVertex3f(-0.1f, 0.0f, -0.1f);   // Bottom back left
        glVertex3f(0.1f, 0.0f, -0.1f);    // Bottom back right
        glVertex3f(0.1f, 0.8f, -0.1f);    // Top back right
        glVertex3f(-0.1f, 0.8f, -0.1f);   // Top back left
        glEnd();

        // Wheel itself (made shorter in depth)
        GLUquadricObj *quadric = gluNewQuadric();
        glRotatef(90, 1, 0, 0);
        
        // Back disk
        gluDisk(quadric, 0, radius, 16, 1);
        
        // Cylinder (tire) - reduced depth from 0.2 to 0.15
        gluCylinder(quadric, radius, radius, 0.15, 16, 1);
        
        // Front disk
        glTranslatef(0, 0, 0.15);
        gluDisk(quadric, 0, radius, 16, 1);
        
        gluDeleteQuadric(quadric);
        glPopMatrix();
    }

    void DrawLandingGear() {
        glColor3fv(wheelColor);
        
        // Adjusted wheel positions and sizes
        // Front wheel (slightly smaller)
        DrawWheel(0.0f, -2.0f, 0.3f, 0.15f);  // Moved forward, reduced radius
        
        // Left wheel (slightly larger)
        DrawWheel(-2.0f, -2.0f, 0.0f, 0.15f);  // Adjusted radius
        
        // Right wheel
        DrawWheel(2.0f, -2.0f, 0.0f, 0.15f);   // Adjusted radius
    }

    void Update() {
        z += speed;
        scale = 1.0 + (z + 60) / 50.0;  // Adjusted scaling
        
        if(z > 15) {
            z = -60;
            scale = 1.0;
        }
    }

    void Draw() {
        DrawBackground();  // Add this line before drawing runway
        DrawRunway();
        DrawAirplane();
        DrawTitle();
    }
};

int main(void) {
    FsOpenWindow(0, 0, 800, 600, 1);
    
    AirplaneAnimation airplane;
    
    glEnable(GL_DEPTH_TEST);

    while(1) {  // Changed from while(running) for simplicity
        int key = FsInkey();
        if(key == FSKEY_ESC) {
            printf("ESC key pressed. Exiting...\n");
            return 0;  // Exit program immediately
        }

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
        
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        
        airplane.Draw();
        airplane.Update();
        
        FsSwapBuffers();
        FsSleep(25);
    }
}

