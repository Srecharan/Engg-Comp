
#include "fssimplewindow.h"
#include <cmath>

class WingDiagram3D {
private:
    const float WING_COLOR[3] = {0.9f, 0.9f, 0.9f};    
    const float SLATS_COLOR[3] = {0.2f, 0.2f, 1.0f};      
    const float AILERON_COLOR[3] = {1.0f, 0.0f, 0.0f};    
    const float FLAPS_COLOR[3] = {1.0f, 1.0f, 0.0f};      
    
    float slatAngle = 0.0f;
    float flapAngle = 0.0f;
    const float ANGLE_STEP = 5.0f;
    const float MAX_ANGLE = 60.0f;

public:
    void DrawMainWing() {
        glColor3f(WING_COLOR[0], WING_COLOR[1], WING_COLOR[2]);
        
        glBegin(GL_POLYGON);
        glVertex3f(100, 300, 0);     
        glVertex3f(450, 180, 0);     
        glVertex3f(450, 260, 0);     
        glVertex3f(100, 400, 0);     
        glEnd();

        glBegin(GL_POLYGON);
        glVertex3f(100, 300, -15);   
        glVertex3f(450, 180, -15);
        glVertex3f(450, 260, -15);
        glVertex3f(100, 400, -15);
        glEnd();

        glBegin(GL_QUAD_STRIP);
        glVertex3f(100, 300, 0);
        glVertex3f(100, 300, -15);
        glVertex3f(450, 180, 0);
        glVertex3f(450, 180, -15);
        glVertex3f(450, 260, 0);
        glVertex3f(450, 260, -15);
        glVertex3f(100, 400, 0);
        glVertex3f(100, 400, -15);
        glVertex3f(100, 300, 0);
        glVertex3f(100, 300, -15);
        glEnd();
    }

    void DrawLeadingEdgeSlats() {
        glColor3f(SLATS_COLOR[0], SLATS_COLOR[1], SLATS_COLOR[2]);
        
        glPushMatrix();
        glTranslatef(100, 297, 0);
        glRotatef(slatAngle, 1, 0, 0);
        glTranslatef(-100, -297, 0);

        glBegin(GL_POLYGON);
        glVertex3f(100, 297, 0);
        glVertex3f(450, 177, 0);     
        glVertex3f(450, 180, 0);
        glVertex3f(100, 300, 0);
        glEnd();

        glBegin(GL_QUAD_STRIP);
        glVertex3f(100, 297, 0);
        glVertex3f(100, 297, -15);
        glVertex3f(450, 177, 0);
        glVertex3f(450, 177, -15);
        glVertex3f(450, 180, 0);
        glVertex3f(450, 180, -15);
        glVertex3f(100, 300, 0);
        glVertex3f(100, 300, -15);
        glEnd();

        glPopMatrix();
    }

    void DrawTrailingEdgeFlaps() {
        glColor3f(FLAPS_COLOR[0], FLAPS_COLOR[1], FLAPS_COLOR[2]);
        
        glPushMatrix();
        glTranslatef(100, 400, 0);
        glRotatef(-flapAngle, 1, 0, 0);
        glTranslatef(-100, -400, 0);

        // Main surface
        glBegin(GL_POLYGON);
        glVertex3f(100, 400, 0);
        glVertex3f(300, 360, 0);
        glVertex3f(300, 330, 0);
        glVertex3f(100, 370, 0);
        glEnd();

        // Add depth
        glBegin(GL_QUAD_STRIP);
        glVertex3f(100, 400, 0);
        glVertex3f(100, 400, -15);
        glVertex3f(300, 360, 0);
        glVertex3f(300, 360, -15);
        glVertex3f(300, 330, 0);
        glVertex3f(300, 330, -15);
        glVertex3f(100, 370, 0);
        glVertex3f(100, 370, -15);
        glEnd();

        glPopMatrix();
    }

    void Draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, 800.0f/600.0f, 0.1f, 1000.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        gluLookAt(200, -150, 150,    
                  300, 300, 0,                   
                  0, 0, 1);                      

        glTranslatef(300, 300, 0);
        glRotatef(65, 0, 0, 1);    
        glRotatef(25, 1, 0, 0);    
        glRotatef(-15, 0, 1, 0);   
        glTranslatef(-300, -300, 0);

        // Handle keyboard input
        int key = FsInkey();
        if(key == FSKEY_UP) {
            slatAngle = fmin(slatAngle + ANGLE_STEP, MAX_ANGLE);
            flapAngle = fmin(flapAngle + ANGLE_STEP, MAX_ANGLE);
        } 
        else if(key == FSKEY_DOWN) {
            slatAngle = fmax(slatAngle - ANGLE_STEP, -MAX_ANGLE);
            flapAngle = fmax(flapAngle - ANGLE_STEP, -MAX_ANGLE);
        }

        DrawMainWing();
        DrawLeadingEdgeSlats();
        DrawTrailingEdgeFlaps();

        glDisable(GL_DEPTH_TEST);
    }
};

int main(void) {
    FsOpenWindow(0, 0, 800, 600, 1);
    
    WingDiagram3D diagram;
    
    while(FsInkey() != FSKEY_ESC) {
        FsPollDevice();
        diagram.Draw();
        FsSwapBuffers();
        FsSleep(16);
    }
    return 0;
}


