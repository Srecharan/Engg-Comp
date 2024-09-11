#include "fssimplewindow.h"
#include <math.h>

const double PI = 3.14159265358979323846;

void DrawCircle(int centerX, int centerY, int radius)
{
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 64; ++i)
    {
        double angle = (double)i * PI / 32.0;
        double x = (double)centerX + cos(angle) * (double)radius;
        double y = (double)centerY + sin(angle) * (double)radius;
        glVertex2i((GLint)x, (GLint)y);  // Cast to GLint to resolve the warning
    }
    glEnd();
}

void DrawCarBody(int x, int y)
{
    // Car body (red)
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + 200, y);
    glVertex2i(x + 200, y - 50);
    glVertex2i(x, y - 50);
    glEnd();

    // Top part (windows)
    glColor3ub(0, 255, 255);
    glBegin(GL_QUADS);
    glVertex2i(x + 50, y - 50);
    glVertex2i(x + 150, y - 50);
    glVertex2i(x + 120, y - 90);
    glVertex2i(x + 80, y - 90);
    glEnd();

    // Windows
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glVertex2i(x + 55, y - 50);
    glVertex2i(x + 95, y - 50);
    glVertex2i(x + 90, y - 85);
    glVertex2i(x + 65, y - 85);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2i(x + 105, y - 50);
    glVertex2i(x + 145, y - 50);
    glVertex2i(x + 135, y - 85);
    glVertex2i(x + 110, y - 85);
    glEnd();
}

void DrawWheels(int x, int y)
{
    // Front wheel
    glColor3ub(0, 0, 0);
    DrawCircle(x + 50, y, 20);

    // Rear wheel
    DrawCircle(x + 150, y, 20);
}

void DrawHeadlights(int x, int y)
{
    // Left headlight
    glColor3ub(255, 255, 0);
    glBegin(GL_TRIANGLES);
    glVertex2i(x - 10, y);
    glVertex2i(x + 10, y);
    glVertex2i(x, y - 20);
    glEnd();

    // Right headlight
    glBegin(GL_TRIANGLES);
    glVertex2i(x + 190, y);
    glVertex2i(x + 210, y);
    glVertex2i(x + 200, y - 20);
    glEnd();
}

void DrawSun(int x, int y)
{
    glColor3ub(255, 255, 0);
    DrawCircle(x, y, 40);

    // Sun rays
    glBegin(GL_LINES);
    for (int i = 0; i < 8; ++i)
    {
        double angle = i * PI / 4;
        glVertex2i(x, y);
        glVertex2i((GLint)(x + cos(angle) * 60), (GLint)(y + sin(angle) * 60));  // Explicit cast to GLint
    }
    glEnd();
}

void DrawCloud(int x, int y)
{
    // A simple cloud made of three circles
    glColor3ub(255, 255, 255);
    DrawCircle(x, y, 20);
    DrawCircle(x + 30, y, 20);
    DrawCircle(x + 15, y - 15, 20);
}

void DrawRoad()
{
    // Road
    glColor3ub(50, 50, 50);
    glBegin(GL_QUADS);
    glVertex2i(0, 600);
    glVertex2i(800, 600);
    glVertex2i(800, 500);
    glVertex2i(0, 500);
    glEnd();

    // Lane lines
    glColor3ub(255, 255, 255);
    for (int i = 0; i < 8; i++)
    {
        glBegin(GL_QUADS);
        glVertex2i(i * 100 + 40, 550);
        glVertex2i(i * 100 + 60, 550);
        glVertex2i(i * 100 + 60, 540);
        glVertex2i(i * 100 + 40, 540);
        glEnd();
    }
}

int main(void)
{
    FsOpenWindow(0, 0, 800, 600, 1);

    for (;;)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sky
        glColor3ub(135, 206, 235);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(800, 0);
        glVertex2i(800, 500);
        glVertex2i(0, 500);
        glEnd();

        // Draw background elements
        DrawSun(700, 100);
        DrawCloud(100, 100);
        DrawCloud(300, 120);
        DrawRoad();

        // Draw car
        DrawCarBody(300, 450);
        DrawWheels(300, 450);
        DrawHeadlights(300, 450);

        FsSwapBuffers();
        FsSleep(20);

        FsPollDevice();
        auto key = FsInkey();
        if (FSKEY_ESC == key)
        {
            break;
        }
    }

    return 0;
}

