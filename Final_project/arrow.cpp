#include "fssimplewindow.h"
#include <vector>
#include <cmath>

struct Arrow {
    double x, y;           
    double speed;          
    int streamline;        
    double scale;         
    bool active;          
};

void DrawArrow(double x, double y, double scale, const double angle = 0.0) {
    glPushMatrix();
    glTranslated(x, y, 0);
    glRotated(angle, 0, 0, 1);
    
    // Arrow body
    glBegin(GL_LINES);
    glVertex2d(-scale, 0);
    glVertex2d(scale, 0);
    
    // Arrow head
    glVertex2d(scale, 0);
    glVertex2d(scale - scale/3, -scale/3);
    glVertex2d(scale, 0);
    glVertex2d(scale - scale/3, scale/3);
    glEnd();
    
    glPopMatrix();
}

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 600;
    FsOpenWindow(16, 16, windowWidth, windowHeight, 1);

    // Initialize arrows
    std::vector<Arrow> arrows;
    const int numStreamlines = 15;
    const int arrowsPerLine = 10;
    
    for(int i = 0; i < numStreamlines; ++i) {
        double baseY = 150 + (i * 25);  
        for(int j = 0; j < arrowsPerLine; ++j) {
            Arrow arrow;
            arrow.x = -50 + (j * 100);  
            arrow.y = baseY;
            arrow.speed = 2.0 + (std::abs(i - numStreamlines/2) * 0.2); 
            arrow.streamline = i;
            arrow.scale = 10.0;
            arrow.active = true;
            arrows.push_back(arrow);
        }
    }

    while(FsCheckWindowOpen()) {
        FsPollDevice();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background

        for(auto& arrow : arrows) {
            // Update position
            arrow.x += arrow.speed;
            
            if(arrow.x > windowWidth + 50) {
                arrow.x = -50;
            }

            double midline = numStreamlines / 2.0;
            double distFromMid = std::abs(arrow.streamline - midline);
            
            
            if(arrow.streamline < midline) {
                glColor4d(0.0, 0.0, 1.0, 0.3 - (distFromMid/numStreamlines));  
            } else {
                glColor4d(1.0, 0.0, 0.0, 0.3 - (distFromMid/numStreamlines));  
            }

            double angleOffset = 0.0;
            if(arrow.x > windowWidth/3 && arrow.x < 2*windowWidth/3) {
                if(arrow.streamline < midline) {
                    angleOffset = -15.0;  
                } else {
                    angleOffset = 15.0;   
                }
            }

            DrawArrow(arrow.x, arrow.y, arrow.scale, angleOffset);
        }

        glColor4d(0.5, 0.0, 0.5, 0.2);  
        for(double angle = 0; angle < 360; angle += 30) {
            double vortexX = windowWidth * 0.7;  
            double vortexY = windowHeight * 0.4;
            double radius = 20;  
            DrawArrow(
                vortexX + radius * cos(angle * M_PI/180),
                vortexY + radius * sin(angle * M_PI/180),
                8.0,
                angle
            );
        }

        FsSwapBuffers();
        FsSleep(20);
    }

    return 0;
}
