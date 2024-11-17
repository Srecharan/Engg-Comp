#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"

void DrawCircle(int x, int y, int rad) {
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 64; i++) {
        double ang = (double)i * 6.28318530718 / 64.0;
        double newX = x + cos(ang) * rad;
        double newY = y + sin(ang) * rad;
        glVertex2d(newX, newY);
    }
    glEnd();
}

class CrankMechanism {
protected:
    // Constants first
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int pixelsPerMeter = 100;
    
    // Link dimensions (in meters)
    const double crankLen = 1.0;    // First shaft
    const double connectorLen = 4.0; // Second shaft
    const double rockerLen = 2.0;    // Third shaft
    const double fixedDistance = 4.0; // Distance between pivots
    
    // Screen setup - calculated after constants
    const int baseX;
    const int baseY;
    
    double rotationAngle;
    
    struct LinkagePoint {
        double x, y;
    };
    
    static const int pathSteps = 72;
    LinkagePoint storedPath[pathSteps];

    void DrawLabel(const char* text, int x, int y) {
        glColor3ub(0, 0, 0);
        glRasterPos2d(x - 15, y + 15);
        YsGlDrawFontBitmap8x12(text);
    }

public:
    CrankMechanism() : 
        baseX(WINDOW_WIDTH/2 - (int)(fixedDistance * pixelsPerMeter)/2 - 50),
        baseY(WINDOW_HEIGHT/2),
        rotationAngle(0.0) {
        GenerateFullPath();
    }
    
    void ComputeMechanismGeometry(double angle, double& x1, double& y1, 
                                 double& x2, double& y2) {
        // P1 position (end of crank)
        x1 = crankLen * cos(angle);
        y1 = crankLen * sin(angle);
        
        // P3 position (fixed rocker pivot)
        double x3 = fixedDistance;
        double y3 = 0;
        
        // Find P2 using circle intersection
        double separation = sqrt(pow(x3 - x1, 2) + pow(y3 - y1, 2));
        
        if (separation > connectorLen + rockerLen || 
            separation < fabs(connectorLen - rockerLen)) {
            return;
        }
        
        double a = (pow(connectorLen, 2) + pow(separation, 2) - 
                   pow(rockerLen, 2)) / (2 * connectorLen * separation);
        if (a > 1.0) a = 1.0;
        if (a < -1.0) a = -1.0;
        double alpha = acos(a);
        
        double baseAngle = atan2(y3 - y1, x3 - x1);
        
        // Take the upper solution
        x2 = x1 + connectorLen * cos(baseAngle + alpha);
        y2 = y1 + connectorLen * sin(baseAngle + alpha);
    }
    
    void GenerateFullPath() {
        for(int i = 0; i < pathSteps; ++i) {
            double angle = i * 6.28318530718 / pathSteps;
            double x1, y1, x2, y2;
            ComputeMechanismGeometry(angle, x1, y1, x2, y2);
            storedPath[i].x = (x1 + x2) / 2.0;
            storedPath[i].y = (y1 + y2) / 2.0;
        }
    }
    
    void Render() {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw path
        glColor3ub(200, 200, 200);
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < pathSteps; ++i) {
            glVertex2d(baseX + storedPath[i].x * pixelsPerMeter, 
                      baseY - storedPath[i].y * pixelsPerMeter);
        }
        glEnd();
        
        // Calculate current positions
        double x1, y1, x2, y2;
        ComputeMechanismGeometry(rotationAngle, x1, y1, x2, y2);
        
        // Draw linkages
        glLineWidth(2);
        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        // Crank
        glVertex2d(baseX, baseY);
        glVertex2d(baseX + x1 * pixelsPerMeter, baseY - y1 * pixelsPerMeter);
        // Connector
        glVertex2d(baseX + x1 * pixelsPerMeter, baseY - y1 * pixelsPerMeter);
        glVertex2d(baseX + x2 * pixelsPerMeter, baseY - y2 * pixelsPerMeter);
        // Rocker
        glVertex2d(baseX + x2 * pixelsPerMeter, baseY - y2 * pixelsPerMeter);
        glVertex2d(baseX + fixedDistance * pixelsPerMeter, baseY);
        glEnd();
        glLineWidth(1);
        
        // Draw fixed points
        glColor3ub(0, 0, 255);
        DrawCircle(baseX, baseY, 5);
        DrawCircle(baseX + (int)(fixedDistance * pixelsPerMeter), baseY, 5);
        
        // Draw moving points
        glColor3ub(0, 255, 0);
        glPointSize(6);
        glBegin(GL_POINTS);
        glVertex2d(baseX + x1 * pixelsPerMeter, baseY - y1 * pixelsPerMeter);
        glVertex2d(baseX + x2 * pixelsPerMeter, baseY - y2 * pixelsPerMeter);
        glVertex2d(baseX + ((x1 + x2)/2) * pixelsPerMeter, 
                  baseY - ((y1 + y2)/2) * pixelsPerMeter);
        glEnd();
        
        // Draw labels
        DrawLabel("P0", baseX, baseY);
        DrawLabel("P1", baseX + (int)(x1 * pixelsPerMeter), 
                      baseY - (int)(y1 * pixelsPerMeter));
        DrawLabel("P2", baseX + (int)(x2 * pixelsPerMeter), 
                      baseY - (int)(y2 * pixelsPerMeter));
        DrawLabel("P3", baseX + (int)(fixedDistance * pixelsPerMeter), baseY);
    }
    
    void Update() {
        rotationAngle += 0.01;
        if(rotationAngle >= 6.28318530718) {
            rotationAngle -= 6.28318530718;
        }
    }
};

int main(void) {
    FsOpenWindow(0, 0, 800, 600, 1);
    CrankMechanism mech;
    
    while(FsInkey() != FSKEY_ESC) {
        mech.Render();
        mech.Update();
        FsSwapBuffers();
        FsSleep(10);
    }
    
    return 0;
}
