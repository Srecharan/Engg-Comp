#include <iostream>
#include <fstream>
#include "fssimplewindow.h"
#include <sstream>

// Bitmap class to handle pixel data and operations
class CharBitmap {
private:
    int width, height;
    char* pixels;

public:
    // Constructor initializes an empty bitmap
    CharBitmap() : width(0), height(0), pixels(nullptr) {}

    // Destructor calls CleanUp to release resources
    ~CharBitmap() {
        CleanUp();
    }

    // CleanUp releases dynamically allocated memory
    void CleanUp() {
        if (pixels != nullptr) {
            delete[] pixels;
            pixels = nullptr;
        }
        width = 0;
        height = 0;
    }

    // Create allocates memory for the bitmap and initializes pixels to 0
    void Create(int w, int h) {
        CleanUp(); // Free any existing memory
        width = w;
        height = h;
        pixels = new char[w * h];
        for (int i = 0; i < w * h; ++i) {
            pixels[i] = 0; // Initialize to black (value 0)
        }
    }

    // SetPixel updates the pixel at (x, y) with the value p
    void SetPixel(int x, int y, char p) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = p;
        }
    }

    // GetPixel retrieves the value of the pixel at (x, y)
    char GetPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixels[y * width + x];
        }
        return 0; // Return black if out of bounds
    }

    // Draw method renders the bitmap as 20x20 squares using OpenGL
    void Draw() const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                char pixel = GetPixel(x, y);
                switch (pixel) {
                    case 0: glColor3ub(0, 0, 0); break;      // Black
                    case 1: glColor3ub(0, 0, 255); break;    // Blue
                    case 2: glColor3ub(255, 0, 0); break;    // Red
                    case 3: glColor3ub(255, 0, 255); break;  // Magenta
                    case 4: glColor3ub(0, 255, 0); break;    // Green
                    case 5: glColor3ub(0, 255, 255); break;  // Cyan
                    case 6: glColor3ub(255, 255, 0); break;  // Yellow
                    case 7: glColor3ub(255, 255, 255); break; // White
                }
                glBegin(GL_QUADS);
                glVertex2i(x * 20, y * 20);
                glVertex2i(x * 20 + 20, y * 20);
                glVertex2i(x * 20 + 20, y * 20 + 20);
                glVertex2i(x * 20, y * 20 + 20);
                glEnd();
            }
        }
    }

    // Save the bitmap data to a file
    bool Save(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << width << " " << height << "\n";
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                file << static_cast<int>(GetPixel(x, y));
            }
            file << "\n";
        }
        return true;
    }

    // Load the bitmap data from a file
    bool Load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        int w, h;
        file >> w >> h;
        Create(w, h); // Create new bitmap with loaded dimensions
        for (int y = 0; y < h; ++y) {
            std::string line;
            file >> line;
            for (size_t x = 0; x < line.size(); ++x) {
                SetPixel(x, y, line[x] - '0');
            }
        }
        return true;
    }
};

// Function to validate and parse user input for bitmap dimensions
bool ParseDimensions(int& width, int& height) {
    std::string input;
    std::getline(std::cin, input);
    std::istringstream ss(input);
    ss >> width >> height;
    return (ss && width > 0 && height > 0 && width <= 64 && height <= 64);
}

// Main function to run the bitmap editor
int main() {
    CharBitmap bitmap;
    int width, height;

    // Request valid dimensions from the user
    do {
        std::cout << "Enter Dimensions (Max 64x64): ";
    } while (!ParseDimensions(width, height));

    // Create bitmap of user-defined size
    bitmap.Create(width, height);
    FsOpenWindow(0, 0, width * 20, height * 20, 1);

    int key;
    while (FSKEY_ESC != (key = FsInkey())) {
        FsPollDevice();

        // Handle mouse input
        int mx, my, lb, mb, rb;
        FsGetMouseState(lb, mb, rb, mx, my);
        int x = mx / 20;
        int y = my / 20;

        // Handle key input for pixel coloring
        if (key >= FSKEY_0 && key <= FSKEY_7) {
            bitmap.SetPixel(x, y, key - FSKEY_0);
        }

        // Save to file if 'S' is pressed
        if (key == FSKEY_S) {
            if (bitmap.Save("pattern.txt")) {
                std::cout << "Bitmap saved to pattern.txt\n";
            }
        }

        // Load from file if 'L' is pressed
        if (key == FSKEY_L) {
            if (bitmap.Load("pattern.txt")) {
                std::cout << "Bitmap loaded from pattern.txt\n";
            }
        }

        // Clear screen and draw bitmap
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bitmap.Draw();

        // Draw the grid
        glColor3ub(255, 255, 255);
        for (int i = 0; i <= width; ++i) {
            glBegin(GL_LINES);
            glVertex2i(i * 20, 0);
            glVertex2i(i * 20, height * 20);
            glEnd();
        }
        for (int i = 0; i <= height; ++i) {
            glBegin(GL_LINES);
            glVertex2i(0, i * 20);
            glVertex2i(width * 20, i * 20);
            glEnd();
        }

        FsSwapBuffers();
        FsSleep(20);
    }

    return 0;
}

