// SLide 3

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
#include "yspng.h"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <cstring>

enum SlideTransition {
    STAY,
    NEXT,
    PREVIOUS
};

// Base Slide class
class Slide {
protected:
    YsSoundPlayer player;
    bool slideRunning = true;

public:
    Slide() {
        player.Start();
    }

    ~Slide() {
        player.End();
    }

    void PrepAudio(YsSoundPlayer::SoundData& audio, const char *audioFile) {
        if (YSOK == audio.LoadWav(audioFile)) {
            player.PreparePlay(audio);
        }
    }

    void PlayBGMAudio(YsSoundPlayer::SoundData& audio) {
        player.PlayBackground(audio);
    }

    void StopAudio(YsSoundPlayer::SoundData& audio) {
        player.Stop(audio);
    }

    void StopAll(YsSoundPlayer::SoundData& audio) {
        player.Stop(audio);
        slideRunning = false;
    }

    bool IsAudioPlaying(YsSoundPlayer::SoundData& audio) {
        return player.IsPlaying(audio);
    }

    void PauseAudio(YsSoundPlayer::SoundData& audio) {
        StopAudio(audio);
    }

    void ResumeAudio(YsSoundPlayer::SoundData& audio) {
        PlayBGMAudio(audio);
    }

    virtual SlideTransition RunSlide() = 0;
};

class Slide_sselvam3 : public Slide {
private:
    struct Box {
        std::string title;
        std::vector<std::string> content;
        double x, y, width, height;
        double opacity;
        double targetX;
        bool isActive;
        bool isExiting;
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
            // YsRawPngDecoder handles its own cleanup
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
            } else if(!box.isExiting && timeCounter >= 5.0) {
                box.isExiting = true;
                timeCounter = 0;
            } else if(box.isExiting) {
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

    void InitializeBoxes() {
        boxes = {
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
                true,
                false
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
                false
            },
            {
                "Takeaway Message",
                {
                    "Wing control surfaces like flaps and",
                    "ailerons manage lift, drag, and",
                    "stability, ensuring safe and efficient",
                    "flight maneuverability."
                },
                -500, 90, 500, 200, 1.0,
                static_cast<double>((windowWidth - 500) / 2),
                false,
                false
            }
        };
    }

    void LoadImages() {
        images.resize(3);
        
        if(YSOK != images[0].png.Decode("slide3.png") ||
           YSOK != images[1].png.Decode("takeoff.png") ||
           YSOK != images[2].png.Decode("landing.png")) {
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
        int imgY = 500;

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
    Slide_sselvam3() : Slide() {
        InitializeBoxes();
        LoadImages();
    }

    SlideTransition RunSlide() override {
        PrepAudio(audio, "crowd.wav");
        PlayBGMAudio(audio);

        while(FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int lb, mb, rb, mx, my;
            int keyCode = FsInkey();
            FsGetMouseState(lb, mb, rb, mx, my);

            if(keyCode == FSKEY_SPACE) {
                if(!isPaused) {
                    PauseAudio(audio);
                    isPaused = true;
                } else {
                    ResumeAudio(audio);
                    isPaused = false;
                }
            }

            player.KeepPlaying();

            if(!isPaused) {
                timeCounter += 0.025;
                UpdateAnimation();
            }

            Render();
            FsSleep(20);

            if(keyCode == FSKEY_LEFT) {
                StopAll(audio);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return PREVIOUS;
            } else if(keyCode == FSKEY_RIGHT) {
                StopAll(audio);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return NEXT;
            }
        }
        return STAY;
    }
};

class Slide_sselvam4 : public Slide {
private:
    bool isPaused = false;
    YsSoundPlayer::SoundData audio;

public:
    Slide_sselvam4() : Slide() {}

    SlideTransition RunSlide() override {
        PrepAudio(audio, "crowd.wav");
        PlayBGMAudio(audio);

        while(FsCheckWindowOpen() && slideRunning) {
            FsPollDevice();
            int keyCode = FsInkey();

            if(keyCode == FSKEY_SPACE) {
                if(!isPaused) {
                    PauseAudio(audio);
                    isPaused = true;
                } else {
                    ResumeAudio(audio);
                    isPaused = false;
                }
            }

            player.KeepPlaying();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glColor3ub(0, 0, 0);
            const char* msg = "Second Slide - Press Left/Right to Navigate";
            glRasterPos2i(300, 300);
            YsGlDrawFontBitmap12x16(msg);
            FsSwapBuffers();

            if(keyCode == FSKEY_LEFT) {
                StopAll(audio);
                return PREVIOUS;
            } else if(keyCode == FSKEY_RIGHT) {
                StopAll(audio);
                return NEXT;
            }

            FsSleep(20);
        }
        return STAY;
    }
};

int main() {
    FsOpenWindow(16, 16, 800, 600, true);

    int currentSlideIndex = 0;
    const int totalSlides = 2;
    Slide* currentSlide = nullptr;

    while(FsCheckWindowOpen()) {
        if(currentSlide == nullptr) {
            if(currentSlideIndex == 0) {
                currentSlide = new Slide_sselvam3();
            } else if(currentSlideIndex == 1) {
                currentSlide = new Slide_sselvam4();  // Using the new second slide
            }
        }

        SlideTransition result = currentSlide->RunSlide();

        if(result == NEXT && currentSlideIndex < totalSlides - 1) {
            delete currentSlide;
            currentSlide = nullptr;
            currentSlideIndex++;
        } else if(result == PREVIOUS && currentSlideIndex > 0) {
            delete currentSlide;
            currentSlide = nullptr;
            currentSlideIndex--;
        } else if(result == STAY) {
            break;
        }
    }

    delete currentSlide;
    return 0;
}

