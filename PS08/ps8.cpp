#include <stdio.h>
#include <algorithm>
#include "fssimplewindow.h"
#include "yspng.h"

class Histogram {
protected:
    unsigned int hist[256];
    bool isInitialized;

public:
    Histogram() : isInitialized(false) {
        for(int i=0; i<256; ++i) {
            hist[i] = 0;
        }
    }

    void Make(const YsRawPngDecoder &png) {
        for(int i=0; i<256; ++i) {
            hist[i] = 0;
        }
        for(int i=0; i<png.wid * png.hei; ++i) {
            auto r = png.rgba[i * 4];
            auto g = png.rgba[i * 4 + 1];
            auto b = png.rgba[i * 4 + 2];
            hist[r] = hist[r] + 1;
            hist[g] = hist[g] + 1;
            hist[b] = hist[b] + 1;
        }
        isInitialized = true;
    }

    void Print(void) const {
        if(!isInitialized) {
            printf("Error: Histogram not initialized. Call Make() first.\n");
            return;
        }

        unsigned int maxNum = 0;
        for(int i = 0; i < 256; ++i) {
            maxNum = std::max(maxNum, hist[i]);
        }

        for(int i = 0; i < 256; ++i) {
            printf("Intensity %3d: ", i);
            int len = hist[i] * 80 / maxNum;
            for(int j = 0; j < len; ++j) {
                printf("#");
            }
            printf("\n");
        }
    }

    void Draw(void) const {
        if(!isInitialized) {
            printf("Error: Histogram not initialized. Call Make() first.\n");
            return;
        }

        unsigned int maxNum = 0;
        for(int i = 0; i < 256; ++i) {
            maxNum = std::max(maxNum, hist[i]);
        }

        int windowWidth, windowHeight;
        FsGetWindowSize(windowWidth, windowHeight);  

        glBegin(GL_LINES);
        for(int i = 0; i < 256; ++i) {
            int height = (hist[i] * 80) / maxNum;
            glVertex2i(i, windowHeight);            
            glVertex2i(i, windowHeight - height);   
        }
        glEnd();
    }
};

int main(void)
{
    FsChangeToProgramDir();

    char filename[256];
    printf("Enter PNG filename: ");
    scanf("%s", filename);

    YsRawPngDecoder png;
    if(YSOK == png.Decode(filename))
    {
        printf("Image Width: %d, Height: %d\n", png.wid, png.hei);
    }
    else
    {
        printf("Failed to open file.\n");
        return 1;
    }
    png.Flip();

    Histogram hist;
    hist.Make(png);
    hist.Print();

    FsOpenWindow(0, 0, png.wid, png.hei, 1);
    for(;;)
    {
        FsPollDevice();
        if(FSKEY_ESC == FsInkey())
        {
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glRasterPos2i(0, png.hei - 1);
        glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
        
        glColor3f(1.0f, 1.0f, 1.0f);  
        hist.Draw();
        
        FsSwapBuffers();
    }

    return 0;
}

