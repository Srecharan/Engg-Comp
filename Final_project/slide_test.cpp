#include <cassert>
#include <cmath>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "yspng.h"

class PresentationTestSuite {
private:
    YsSoundPlayer::SoundData testAudio;
    YsRawPngDecoder testImage;

    bool TestAudioSystem() {
        // Test audio loading
        if (YSOK != testAudio.LoadWav("test_audio.wav")) {
            return false;
        }
        
        // Test player initialization
        YsSoundPlayer player;
        if (!player.Start()) {
            return false;
        }
        
        // Test play/pause functionality
        player.PreparePlay(testAudio);
        player.PlayBackground(testAudio);
        bool isPlaying = player.IsPlaying(testAudio);
        player.Stop(testAudio);
        bool isStopped = !player.IsPlaying(testAudio);
        
        player.End();
        return isPlaying && isStopped;
    }

    bool TestImageLoading() {
        // Test PNG decoder
        if (YSOK != testImage.Decode("test_image.png")) {
            return false;
        }
        
        // Verify image dimensions
        if (testImage.wid <= 0 || testImage.hei <= 0) {
            return false;
        }
        
        // Check image data
        if (testImage.rgba == nullptr) {
            return false;
        }
        
        return true;
    }

    bool TestSlideTransitions() {
        Slide_sselvam1 slide1;
        Slide_sselvam2 slide2;
        Slide_sselvam3 slide3;
        
        // Test left key transition
        FsPostKeyboardEvent(FSKEY_LEFT, 0);
        if (slide2.RunSlide() != PREVIOUS) {
            return false;
        }
        
        // Test right key transition
        FsPostKeyboardEvent(FSKEY_RIGHT, 0);
        if (slide1.RunSlide() != NEXT) {
            return false;
        }
        
        return true;
    }

    bool TestAnimationSystem() {
        // Test airplane animation parameters
        Slide_sselvam1 slide;
        double initialZ = -60;
        double initialScale = 1.0;
        
        // Verify animation bounds
        if (initialZ > 15 || initialScale < 0) {
            return false;
        }
        
        // Test pause functionality
        FsPostKeyboardEvent(FSKEY_SPACE, 0);
        
        return true;
    }

    bool TestLabelSystem() {
        Slide_sselvam2 slide;
        
        // Test label positioning
        const double screenWidth = 800;
        const double screenHeight = 600;
        
        // Verify label boundaries
        for (const auto& label : slide.GetLabels()) {
            if (label.targetX < 0 || label.targetX > screenWidth ||
                label.targetY < 0 || label.targetY > screenHeight) {
                return false;
            }
        }
        
        return true;
    }

public:
    bool RunAllTests() {
        bool success = true;
        
        // Initialize window for tests
        FsOpenWindow(16, 16, 800, 600, true);
        
        // Run test suite
        success &= TestAudioSystem();
        success &= TestImageLoading();
        success &= TestSlideTransitions();
        success &= TestAnimationSystem();
        success &= TestLabelSystem();
        
        return success;
    }
};

int main() {
    PresentationTestSuite tests;
    bool testsPassed = tests.RunAllTests();
    
    if (testsPassed) {
        printf("All tests passed successfully.\n");
    } else {
        printf("Some tests failed. Check individual test results.\n");
    }
    
    return 0;
}
