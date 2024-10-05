#include <iostream>
#include <cmath>
#include "fssimplewindow.h"

const double tolerance = 1e-6;

class Equation {
public:
    double a, b, c;

    Equation() : a(0), b(0), c(0) {}

    void Plot(float R, float G, float B) const {
        glColor3f(R, G, B);
        glBegin(GL_LINES);

        double x1 = -30, x2 = 30;
        double y1, y2;

        if (std::fabs(b) > tolerance) {  
            y1 = (c - a * x1) / b;
            y2 = (c - a * x2) / b;
        } else if (std::fabs(a) > tolerance) {  
            x1 = x2 = c / a;
            y1 = -30;
            y2 = 30;
        } else { 
            return;
        }

        glVertex2i(300 + x1 * 10, 300 - y1 * 10);
        glVertex2i(300 + x2 * 10, 300 - y2 * 10);

        glEnd();
    }
};

class SimultaneousEquation {
public:
    Equation eqn[2];

    bool Solve(double &x, double &y) const {
        double D = eqn[0].a * eqn[1].b - eqn[0].b * eqn[1].a;
        if (std::fabs(D) < tolerance) return false;

        x = (eqn[1].b * eqn[0].c - eqn[0].b * eqn[1].c) / D;
        y = (eqn[0].a * eqn[1].c - eqn[1].a * eqn[0].c) / D;
        return true;
    }

    void Plot() const {
        eqn[0].Plot(1.0f, 0.0f, 0.0f);  // Red
        eqn[1].Plot(0.0f, 0.0f, 1.0f);  // Blue
    }
};

class Axes {
public:
    void Draw() {
        glColor3f(0.7f, 0.7f, 0.7f); 
        glBegin(GL_LINES);
        for (int i = 0; i <= 600; i += 10) {
            glVertex2i(i, 0);
            glVertex2i(i, 600);
            glVertex2i(0, i);
            glVertex2i(600, i);
        }
        glEnd();

        // X- and Y-axes in black
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        glVertex2i(300, 0);
        glVertex2i(300, 600);
        glVertex2i(0, 300);
        glVertex2i(600, 300);
        glEnd();
    }
};

int main(void) {
    SimultaneousEquation eqn;
    Axes axes;
    double x, y;

    std::cout << "ax+by=c\n";
    std::cout << "dx+ey=f\n";
    std::cout << "Enter a b c d e f: ";
    std::cin >> eqn.eqn[0].a >> eqn.eqn[0].b >> eqn.eqn[0].c >> eqn.eqn[1].a >> eqn.eqn[1].b >> eqn.eqn[1].c;

    if (eqn.Solve(x, y)) {
        std::cout << "x=" << x << " y=" << y << '\n';
    } else {
        std::cout << "No solution.\n";
    }

    FsOpenWindow(0, 0, 600, 600, 1);
    for (;;) {
        FsPollDevice();
        if (FSKEY_ESC == FsInkey()) break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw axes and grid
        axes.Draw();

        // Plot the equations
        eqn.Plot();

        FsSwapBuffers();
    }

    return 0;
}

