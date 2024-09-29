#include "fssimplewindow.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>  
#include <cstdio>

#ifndef M_PI
#define M_PI 3.141
#endif

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const double PLAYER_RADIUS = 25.0;  
const int STAR_RADIUS = 10;  
const int NUM_STARS = 10;
const int NUM_OBSTACLES = 3;
const int TARGET_STARS = 10;  
const int OBSTACLE_VELOCITY = 3;  
bool allStarsCollected = false;  

enum GameState { PLAYING, GAME_OVER, FADING };

struct Star {
    double x, y;
    bool collected;
};

struct Obstacle {
    int x, y, width, height;
    int velX, velY;  
};

GameState gameState = PLAYING;
double playerX = WINDOW_WIDTH / 2, playerY = WINDOW_HEIGHT / 2;  
std::vector<Star> stars;
std::vector<Obstacle> obstacles;
int collectedStars = 0;
float playerAlpha = 1.0f;  
bool pacManIsBlue = false;  
float red = 1.0f, green = 1.0f, blue = 0.0f;  // Initial Pac-Man color (Yellow)
float colorTransitionSpeed = 0.01f;  // Speed of color transition
int blueDuration = 0;  // Timer to track how long Pac-Man stays blue

// Function to smoothly transition Pac-Man's color towards a target color
void TransitionPacManColor(float targetRed, float targetGreen, float targetBlue) {
    if (red < targetRed) red += colorTransitionSpeed;
    if (red > targetRed) red -= colorTransitionSpeed;

    if (green < targetGreen) green += colorTransitionSpeed;
    if (green > targetGreen) green -= colorTransitionSpeed;

    if (blue < targetBlue) blue += colorTransitionSpeed;
    if (blue > targetBlue) blue -= colorTransitionSpeed;
}

void DrawPacMan(double x, double y, double radius, float alpha) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(red, green, blue, alpha);  // Dynamic Pac-Man color

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x, y);
    for (int i = 30; i <= 330; ++i) {
        double angle = i * M_PI / 180.0;
        double px = x + radius * cos(angle);
        double py = y + radius * sin(angle);
        glVertex2d(px, py);
    }
    glEnd();

    glColor4f(0.0, 0.0, 0.0, alpha);  
    double eyeX = x + radius / 3.0;
    double eyeY = y - radius / 2.0;
    glBegin(GL_QUADS);
    glVertex2d(eyeX - 3, eyeY - 3);
    glVertex2d(eyeX + 3, eyeY - 3);
    glVertex2d(eyeX + 3, eyeY + 3);
    glVertex2d(eyeX - 3, eyeY + 3);
    glEnd();

    glDisable(GL_BLEND);  
}

void DrawCircle(double x, double y, double radius, int numSegments = 100) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x, y);
    for (int i = 0; i <= numSegments; ++i) {
        double angle = 2.0 * M_PI * i / numSegments;
        double px = x + radius * cos(angle);
        double py = y + radius * sin(angle);
        glVertex2d(px, py);
    }
    glEnd();
}

void InitializeStars() {
    srand(time(nullptr));
    for (int i = 0; i < NUM_STARS; ++i) {
        Star star;
        star.x = rand() % (WINDOW_WIDTH - 2 * STAR_RADIUS) + STAR_RADIUS;
        star.y = rand() % (WINDOW_HEIGHT - 2 * STAR_RADIUS) + STAR_RADIUS;
        star.collected = false;
        stars.push_back(star);
    }
}

void InitializeObstacles() {
    srand(time(nullptr));
    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        Obstacle obs;
        obs.x = rand() % (WINDOW_WIDTH - 100) + 50;
        obs.y = rand() % (WINDOW_HEIGHT - 100) + 50;
        obs.width = 30;
        obs.height = 30;
        obs.velX = (rand() % 2 == 0) ? OBSTACLE_VELOCITY : -OBSTACLE_VELOCITY;  
        obs.velY = (rand() % 2 == 0) ? OBSTACLE_VELOCITY : -OBSTACLE_VELOCITY;  
        obstacles.push_back(obs);
    }
}

void DrawStars() {
    for (const auto &star : stars) {
        if (!star.collected) {
            glColor3ub(0, 0, 255);
            DrawCircle(star.x, star.y, STAR_RADIUS);  
        }
    }
}

void DrawGhost(int x, int y, int width, int height) {
    glColor3ub(255, 0, 0);  
    glBegin(GL_POLYGON);
    glVertex2i(x, y + height / 2);
    for (int i = 0; i <= 180; ++i) {
        double angle = i * M_PI / 180;
        double px = x + width / 2 + (width / 2) * cos(angle);
        double py = y + height / 2 + (height / 2) * sin(angle);
        glVertex2d(px, py);
    }
    glVertex2i(x + width, y);
    glVertex2i(x + width - width / 4, y - height / 4);
    glVertex2i(x + width / 2, y);
    glVertex2i(x + width / 4, y - height / 4);
    glVertex2i(x, y);
    glEnd();

    glColor3ub(255, 255, 255);
    DrawCircle(x + width / 3, y + height / 3, 6);
    DrawCircle(x + 2 * width / 3, y + height / 3, 6);

    glColor3ub(0, 0, 255);
    DrawCircle(x + width / 3, y + height / 3, 3);
    DrawCircle(x + 2 * width / 3, y + height / 3, 3);
}

bool CheckCollision(double x1, double y1, double size1, double x2, double y2, double size2) {
    return (std::abs(x1 - x2) < (size1 + size2) / 2) && (std::abs(y1 - y2) < (size1 + size2) / 2);
}

bool CheckObstacleCollision(double x, double y) {
    for (const auto &obs : obstacles) {
        if (x + PLAYER_RADIUS / 2 > obs.x && x - PLAYER_RADIUS / 2 < obs.x + obs.width &&
            y + PLAYER_RADIUS / 2 > obs.y && y - PLAYER_RADIUS / 2 < obs.y + obs.height) {
            return true;
        }
    }
    return false;
}

void StartFading() {
    gameState = FADING;
}

void UpdateFading() {
    if (playerAlpha > 0.0f) {
        playerAlpha -= 0.01f;  
    } else {
        gameState = GAME_OVER;  
    }
}

void UpdateObstaclePosition() {
    for (auto &obs : obstacles) {
        obs.x += obs.velX;
        obs.y += obs.velY;

        if (obs.x <= 0 || obs.x + obs.width >= WINDOW_WIDTH) {
            obs.velX *= -1;
        }
        if (obs.y <= 0 || obs.y + obs.height >= WINDOW_HEIGHT) {
            obs.velY *= -1;
        }
    }
}

// Update Pac-Man's color based on whether it should transition to blue or back to yellow
void UpdatePacManColor() {
    if (pacManIsBlue) {
        TransitionPacManColor(0.0f, 0.0f, 1.0f);  // Transition to blue
        blueDuration++;  // Increment blue duration timer
        if (blueDuration > 200) {  // After some time, transition back to yellow
            pacManIsBlue = false;
            blueDuration = 0;  // Reset the blue duration timer
        }
    } else {
        TransitionPacManColor(1.0f, 1.0f, 0.0f);  // Transition back to yellow
    }
}

void UpdateGame() {
    if (gameState == PLAYING) {
        if (FsGetKeyState(FSKEY_LEFT) && playerX - PLAYER_RADIUS / 2 > 0) playerX -= 5;
        if (FsGetKeyState(FSKEY_RIGHT) && playerX + PLAYER_RADIUS / 2 < WINDOW_WIDTH) playerX += 5;
        if (FsGetKeyState(FSKEY_UP) && playerY - PLAYER_RADIUS / 2 > 0) playerY -= 5;
        if (FsGetKeyState(FSKEY_DOWN) && playerY + PLAYER_RADIUS / 2 < WINDOW_HEIGHT) playerY += 5;

        for (auto &star : stars) {
            if (!star.collected && CheckCollision(playerX, playerY, PLAYER_RADIUS, star.x, star.y, STAR_RADIUS)) {
                star.collected = true;
                collectedStars++;
                pacManIsBlue = true;  // Turn Pac-Man blue
            }
        }

        for (const auto &obs : obstacles) {
            if (CheckObstacleCollision(playerX, playerY)) {
                StartFading();  
            }
        }

        UpdateObstaclePosition();
        UpdatePacManColor();  // Update the color of Pac-Man

        if (collectedStars >= TARGET_STARS && !allStarsCollected) {
            allStarsCollected = true;  
        }
    } else if (gameState == FADING) {
        UpdateFading();  
    }
}

int main(void) {
    FsOpenWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);
    InitializeStars();
    InitializeObstacles();

    for (;;) {
        FsPollDevice();
        int key = FsInkey();
        if (key == FSKEY_ESC) break;

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        if (gameState != GAME_OVER) {
            DrawPacMan(playerX, playerY, PLAYER_RADIUS, playerAlpha);
        }

        if (gameState != FADING && gameState != GAME_OVER) {
            DrawStars();
            for (const auto &obs : obstacles) {
                DrawGhost(obs.x, obs.y, obs.width, obs.height);
            }
        }

        UpdateGame();

        FsSwapBuffers();
        FsSleep(25);

        if (allStarsCollected && gameState != FADING) {
            printf("YOU WIN! Stars collected: %d\n", collectedStars);
            FsSleep(2000);
            exit(0);
        }

        if (gameState == GAME_OVER) {
            printf("GAME OVER: Pac-Man has disappeared! Stars collected: %d\n", collectedStars);
            FsSleep(2000);
            exit(0);
        }
    }

    return 0;
}

