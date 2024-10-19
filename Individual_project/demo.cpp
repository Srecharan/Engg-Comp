#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <string>
#include <cstring>  // For strlen

int gameTimer = 0;  // Timer in seconds
int startTime = 0;
int score = 0;
bool collide = false;
int vehicleX = 400, vehicleY = 70;
int ovehicleX[4], ovehicleY[4];
int divx1 = 333, divx2 = 467, divy = 4, movd = 0;
int currentLane = 1;
const int ROAD_LEFT = 200;
const int ROAD_RIGHT = 600;
const int LANE_WIDTH = 133;

bool allFuelCollected = false;
bool isGameOver = false;
bool isExploding = false;
bool introScreen = true;
int stickManX = 100;
int stickManY = 400;
int introCar = 600;
double titleAlpha = 0;
bool showPressKey = true;
int blinkTimer = 0;
int introPhase = 0;
int explosionRadius = 0;
int lastScoreDeductionTime = 0;
int initialFuel = 1000;

const int letterWidth = 40;
const int letterHeight = 60;
const int letterSpacing = 10;

const int NUM_SCENERY = 10;
struct Scenery {
    int x, y;
    int type;
    int height;
} leftScenery[NUM_SCENERY / 2], rightScenery[NUM_SCENERY / 2];

struct FuelCan {
    int x, y;
    bool active;
    float rotation;
};
FuelCan fuelCans[36];

YsSoundPlayer soundPlayer;
YsSoundPlayer::SoundData laneChangeSound;
YsSoundPlayer::SoundData collisionSound;

void drawThickLine(int x1, int y1, int x2, int y2, int thickness) {
    glLineWidth(thickness);
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
    glLineWidth(1);
}

void drawCustomLetter(char letter, int x, int y, int size) {
    int s = size;
    glColor3ub(255, 0, 0);
    
    switch(letter) {
        case 'F':
            drawThickLine(x, y, x, y+s*2, 3);
            drawThickLine(x, y, x+s, y, 3);
            drawThickLine(x, y+s, x+s*0.7, y+s, 3);
            break;
        case 'A':
            drawThickLine(x, y+s*2, x+s/2, y, 3);
            drawThickLine(x+s/2, y, x+s, y+s*2, 3);
            drawThickLine(x+s*0.3, y+s, x+s*0.7, y+s, 3);
            break;
        case 'S':
            drawThickLine(x+s, y, x, y+s, 3);
            drawThickLine(x, y+s, x+s, y+s*2, 3);
            break;
        case 'T':
            drawThickLine(x, y, x+s, y, 3);
            drawThickLine(x+s/2, y, x+s/2, y+s*2, 3);
            break;
        case 'N':
            drawThickLine(x, y+s*2, x, y, 3);
            drawThickLine(x, y, x+s, y+s*2, 3);
            drawThickLine(x+s, y+s*2, x+s, y, 3);
            break;
        case 'D':
            drawThickLine(x, y, x, y+s*2, 3);
            drawThickLine(x, y, x+s*0.7, y+s*0.3, 3);
            drawThickLine(x+s*0.7, y+s*0.3, x+s*0.7, y+s*1.7, 3);
            drawThickLine(x+s*0.7, y+s*1.7, x, y+s*2, 3);
            break;
        case 'U':
            drawThickLine(x, y, x, y+s*2, 3);
            drawThickLine(x, y+s*2, x+s, y+s*2, 3);
            drawThickLine(x+s, y+s*2, x+s, y, 3);
            break;
        case 'R':
            drawThickLine(x, y, x, y+s*2, 3);
            drawThickLine(x, y, x+s, y, 3);
            drawThickLine(x+s, y, x+s, y+s, 3);
            drawThickLine(x, y+s, x+s, y+s, 3);
            drawThickLine(x+s*0.5, y+s, x+s, y+s*2, 3);
            break;
        case 'I':
            drawThickLine(x+s/2, y, x+s/2, y+s*2, 3);
            break;
        case 'O':
            drawThickLine(x, y, x+s, y, 3);
            drawThickLine(x+s, y, x+s, y+s*2, 3);
            drawThickLine(x, y+s*2, x+s, y+s*2, 3);
            drawThickLine(x, y, x, y+s*2, 3);
            break;
    }
}

void drawTextAlt(int x, int y, const char *str) {
    glColor3ub(255, 255, 255);
    glRasterPos2i(x, y);

    for (int i = 0; str[i] != '\0'; i++) {
        for (int bit = 0; bit < 8; ++bit) {
            if (str[i] & (1 << bit)) {
                glBegin(GL_POINTS);
                glVertex2i(x + bit * 8, y);
                glEnd();
            }
        }
        x += 10;
    }
}

void drawText(const char str[], int x, int y) {
    drawTextAlt(x, y, str);
}

void drawTitle(double alpha) {
    glColor4ub(255, 0, 0, (unsigned char)(alpha * 255));
    const char* title = "NOT FAST AND NOT FURIOUS";
    int textWidth = strlen(title) * 32;
    
    int centerX = 400;
    int centerY = 200;
    
    glRasterPos2i(centerX - textWidth/2, centerY);
    YsGlDrawFontBitmap32x48(title);
    
    const char* subtitle = "DRIVE FAST, LIVE FASTER";
    textWidth = strlen(subtitle) * 24;
    glRasterPos2i(centerX - textWidth/2, centerY + 70);
    YsGlDrawFontBitmap20x32(subtitle);
    
    if(showPressKey && alpha > 0.9) {
        const char* pressText = "Press SPACE to start";
        textWidth = strlen(pressText) * 20;
        glColor4ub(255, 255, 255, (unsigned char)(alpha * 255));
        glRasterPos2i(centerX - textWidth/2, 500);
        YsGlDrawFontBitmap16x24(pressText);
    }
}

void drawTimer(int seconds) {
    char timeStr[64];
    sprintf(timeStr, "Time: %d s", seconds);
    glColor3ub(255, 255, 0);
    glRasterPos2i(600, 30);
    YsGlDrawFontBitmap16x24(timeStr);
}

void drawScore(const char* str) {
    glColor3ub(255, 255, 0);
    glRasterPos2i(20, 30);
    YsGlDrawFontBitmap16x24(str);
}

void drawStickMan(int x, int y) {
    glColor3ub(255, 255, 255);
    glLineWidth(3);
    
    for(int i = 2; i >= 0; i--) {
        if(i > 0) {
            glColor4ub(255, 255, 255, 100);
        } else {
            glColor3ub(255, 255, 255);
        }
        
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < 32; j++) {
            double angle = (double)j * 6.28318530718 / 32.0;
            double px = x + cos(angle) * (15 + i*2);
            double py = y - 60 + sin(angle) * (15 + i*2);
            glVertex2d(px, py);
        }
        glEnd();
    }
    
    double legAngle = sin(stickManX * 0.1) * 0.3;
    
    glBegin(GL_LINES);
    glVertex2i(x, y-45);
    glVertex2i(x, y);
    
    double armAngle = cos(stickManX * 0.1) * 0.3;
    glVertex2i(x, y-35);
    glVertex2i(x-20 * cos(armAngle), y-20);
    glVertex2i(x, y-35);
    glVertex2i(x+20 * cos(-armAngle), y-20);
    
    glVertex2i(x, y);
    glVertex2i(x-20 * cos(legAngle), y+30);
    glVertex2i(x, y);
    glVertex2i(x+20 * cos(-legAngle), y+30);
    glEnd();
    
    glLineWidth(1);
}

void drawIntroCar(int x, int y) {
    const int carWidth = 80;
    const int carHeight = 50;
    
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(x-carWidth+5, y+carHeight+5);
    glVertex2i(x+carWidth+5, y+carHeight+5);
    glVertex2i(x+carWidth+5, y+carHeight+15);
    glVertex2i(x-carWidth+5, y+carHeight+15);
    glEnd();
    
    glBegin(GL_QUADS);
    for(int i = 0; i < carHeight; i++) {
        float intensity = 1.0f - (float)i/carHeight * 0.3f;
        glColor3ub((int)(255*intensity), 0, 0);
        glVertex2i(x-carWidth, y+i);
        glVertex2i(x+carWidth, y+i);
        glVertex2i(x+carWidth, y+i+1);
        glVertex2i(x-carWidth, y+i+1);
    }
    glEnd();
    
    glColor3ub(180, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(x-carWidth*0.7, y);
    glVertex2i(x+carWidth*0.7, y);
    glVertex2i(x+carWidth*0.5, y-carHeight);
    glVertex2i(x-carWidth*0.5, y-carHeight);
    glEnd();
    
    glBegin(GL_QUADS);
    for(int i = 0; i < carHeight; i++) {
        float intensity = 0.5f + (float)i/carHeight * 0.5f;
        glColor3ub(100*intensity, 149*intensity, 237*intensity);
        glVertex2i(x-carWidth*0.65+i/2, y-i);
        glVertex2i(x+carWidth*0.65-i/2, y-i);
        glVertex2i(x+carWidth*0.65-i/2, y-i+1);
        glVertex2i(x-carWidth*0.65+i/2, y-i+1);
    }
    glEnd();
    
    for(int side = -1; side <= 1; side += 2) {
        glColor3ub(192, 192, 192);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 32; i++) {
            double angle = (double)i * 6.28318530718 / 32.0;
            glVertex2d(x + side * carWidth*0.8 + cos(angle) * 20, 
                      y + carHeight + sin(angle) * 20);
        }
        glEnd();
        
        glColor3ub(30, 30, 30);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 32; i++) {
            double angle = (double)i * 6.28318530718 / 32.0;
            glVertex2d(x + side * carWidth*0.8 + cos(angle) * 15, 
                      y + carHeight + sin(angle) * 15);
        }
        glEnd();
    }
}

void drawCountdownNumber(int number, int x, int y) {
    glColor3ub(255, 255, 255);
    char numStr[2];
    sprintf(numStr, "%d", number);
    
    glRasterPos2i(x - 20, y + 20);
    YsGlDrawFontBitmap32x48(numStr);
}

void drawRaceText(int x, int y) {
    const char* text = "RACE!";
    glColor3ub(255, 255, 255);
    
    int textWidth = strlen(text) * 32;
    glRasterPos2i(x - textWidth/2, y + 20);
    YsGlDrawFontBitmap32x48(text);
}

void initScenery() {
    for(int i = 0; i < NUM_SCENERY/2; ++i) {
        leftScenery[i].x = rand() % 150;
        leftScenery[i].y = i * 120;
        leftScenery[i].type = rand() % 2;
        leftScenery[i].height = 50 + rand() % 100;
    }
    
    for(int i = 0; i < NUM_SCENERY/2; ++i) {
        rightScenery[i].x = ROAD_RIGHT + 50 + rand() % 150;
        rightScenery[i].y = i * 120;
        rightScenery[i].type = rand() % 2;
        rightScenery[i].height = 50 + rand() % 100;
    }
}

void drawTree(int x, int y) {
    glColor3ub(139, 69, 19);
    glBegin(GL_QUADS);
    glVertex2i(x-5, y);
    glVertex2i(x+5, y);
    glVertex2i(x+5, y+30);
    glVertex2i(x-5, y+30);
    glEnd();
    
    glColor3ub(34, 139, 34);
    glBegin(GL_TRIANGLES);
    glVertex2i(x-20, y);
    glVertex2i(x+20, y);
    glVertex2i(x, y-40);
    glEnd();
}

void drawBuilding(int x, int y, int height) {
    glColor3ub(169, 169, 169);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x+40, y);
    glVertex2i(x+40, y-height);
    glVertex2i(x, y-height);
    
    glColor3ub(255, 255, 0);
    for(int i = 10; i < height-10; i += 20) {
        for(int j = 5; j < 35; j += 15) {
            glVertex2i(x+j, y-i);
            glVertex2i(x+j+10, y-i);
            glVertex2i(x+j+10, y-i-10);
            glVertex2i(x+j, y-i-10);
        }
    }
    glEnd();
}

void drawScenery() {
    glBegin(GL_QUADS);
    glColor3ub(60, 179, 113);
    glVertex2i(0, 0);
    glVertex2i(ROAD_LEFT, 0);
    glVertex2i(ROAD_LEFT, 600);
    glVertex2i(0, 600);
    
    glVertex2i(ROAD_RIGHT, 0);
    glVertex2i(800, 0);
    glVertex2i(800, 600);
    glVertex2i(ROAD_RIGHT, 600);
    glEnd();

    for(int i = 0; i < NUM_SCENERY / 2; ++i) {
        leftScenery[i].y = (leftScenery[i].y + 3) % 600;
        rightScenery[i].y = (rightScenery[i].y + 3) % 600;
        
        if(leftScenery[i].type == 0) {
            drawTree(leftScenery[i].x, leftScenery[i].y);
        } else {
            drawBuilding(leftScenery[i].x, leftScenery[i].y, leftScenery[i].height);
        }
        
        if(rightScenery[i].type == 0) {
            drawTree(rightScenery[i].x, rightScenery[i].y);
        } else {
            drawBuilding(rightScenery[i].x, rightScenery[i].y, rightScenery[i].height);
        }
    }
}

void LoadSounds() {
    if (YSOK != laneChangeSound.LoadWav("lane_change.wav")) {
        printf("Error loading lane_change.wav - continuing without sound\n");
    }
    
    if (YSOK != collisionSound.LoadWav("collision.wav")) {
        printf("Error loading collision.wav - continuing without sound\n");
    }
}

void PlayLaneChangeSound() {
    soundPlayer.PlayOneShot(laneChangeSound);
}

void PlayCollisionSound() {
    soundPlayer.PlayOneShot(collisionSound);
}

void ovpos() {
    const int MIN_VERTICAL_GAP = 250;
    int lastY = 800;
    
    for(int i = 0; i < 4; i++) {
        ovehicleX[i] = ROAD_LEFT + (rand() % 3) * LANE_WIDTH + LANE_WIDTH / 2;
        ovehicleY[i] = lastY + MIN_VERTICAL_GAP + (rand() % 100);
        lastY = ovehicleY[i];
    }
}

void drawRoad() {
    glBegin(GL_QUADS);
    glColor3ub(128, 128, 128);
    glVertex2i(ROAD_LEFT, 600);
    glVertex2i(ROAD_LEFT, 0);
    glVertex2i(ROAD_RIGHT, 0);
    glVertex2i(ROAD_RIGHT, 600);
    glEnd();
}

void drawDividers() {
    glPushMatrix();
    glTranslatef(0, movd, 0);
    
    for (int div = 0; div < 2; div++) {
        int x = (div == 0) ? divx1 : divx2;
        for (int i = 0; i <= 15; i++) {
            glColor3ub(255, 255, 255);
            glBegin(GL_QUADS);
            glVertex2i(x - 5, divy * 15 * i + 18);
            glVertex2i(x - 5, divy * 15 * i - 18);
            glVertex2i(x + 5, divy * 15 * i - 18);
            glVertex2i(x + 5, divy * 15 * i + 18);
            glEnd();
        }
    }
    glPopMatrix();
}

void drawVehicle() {
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(vehicleX - 25, vehicleY + 20);
    glVertex2i(vehicleX - 25, vehicleY - 15);
    glVertex2i(vehicleX + 25, vehicleY - 15);
    glVertex2i(vehicleX + 25, vehicleY + 20);
    
    glColor3ub(200, 0, 0);
    glVertex2i(vehicleX - 20, vehicleY - 15);
    glVertex2i(vehicleX - 15, vehicleY - 30);
    glVertex2i(vehicleX + 15, vehicleY - 30);
    glVertex2i(vehicleX + 20, vehicleY - 15);
    glEnd();
    
    glColor3ub(100, 149, 237);
    glBegin(GL_QUADS);
    glVertex2i(vehicleX - 18, vehicleY - 16);
    glVertex2i(vehicleX - 13, vehicleY - 28);
    glVertex2i(vehicleX + 13, vehicleY - 28);
    glVertex2i(vehicleX + 18, vehicleY - 16);
    glEnd();
    
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 32; i++) {
        double angle = (double)i * 6.28318530718 / 32.0;
        glVertex2d(vehicleX - 20 + cos(angle) * 8, vehicleY + 20 + sin(angle) * 8);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for(int i = 0; i < 32; i++) {
        double angle = (double)i * 6.28318530718 / 32.0;
        glVertex2d(vehicleX + 20 + cos(angle) * 8, vehicleY + 20 + sin(angle) * 8);
    }
    glEnd();
}

void initFuelCans() {
    const int MIN_GAP = 120;
    int lastY = 600;
    
    for(int i = 0; i < 36; i++) {
        fuelCans[i].x = ROAD_LEFT + (rand() % 3) * LANE_WIDTH + LANE_WIDTH / 2;
        fuelCans[i].y = lastY + MIN_GAP + (rand() % 30);
        fuelCans[i].active = true;
        fuelCans[i].rotation = 0;
        lastY = fuelCans[i].y;
    }
}

void drawFuelCan(int x, int y, float rotation, bool active) {
    if (!active) return;
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotation, 0, 0, 1);
    
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(-15, -20);
    glVertex2i(15, -20);
    glVertex2i(15, 20);
    glVertex2i(-15, 20);
    glEnd();
    
    glColor3ub(128, 128, 128);
    glBegin(GL_QUADS);
    glVertex2i(-5, -25);
    glVertex2i(5, -25);
    glVertex2i(5, -20);
    glVertex2i(-5, -20);
    glEnd();
    
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2i(-5, 0);
    glVertex2i(5, 0);
    glVertex2i(0, -5);
    glVertex2i(0, 5);
    glEnd();
    
    glPopMatrix();
}

void updateAndDrawFuelCans() {
    const int FUEL_SPEED = 4;
    const int COLLECTION_DISTANCE = 30;
    
    for(int i = 0; i < 36; i++) {
        if (fuelCans[i].active) {
            fuelCans[i].y -= FUEL_SPEED;
            fuelCans[i].rotation += 2;
            
            if (abs(fuelCans[i].x - vehicleX) < COLLECTION_DISTANCE && 
                abs(fuelCans[i].y - vehicleY) < COLLECTION_DISTANCE) {
                fuelCans[i].active = false;
                score += 100;
                PlayLaneChangeSound();
            }
            
            drawFuelCan(fuelCans[i].x, fuelCans[i].y, fuelCans[i].rotation, fuelCans[i].active);
            
            if (fuelCans[i].y < -25) {
                int maxY = -25;
                for (int j = 0; j < 36; j++) {
                    if (fuelCans[j].y > maxY) {
                        maxY = fuelCans[j].y;
                    }
                }
                
                int newLane = rand() % 3;
                bool tooClose;
                do {
                    tooClose = false;
                    fuelCans[i].y = maxY + 120 + (rand() % 50);
                    fuelCans[i].x = ROAD_LEFT + newLane * LANE_WIDTH + LANE_WIDTH / 2;
                    
                    for (int j = 0; j < 45; j++) {
                        if (i != j && fuelCans[j].active && 
                            abs(fuelCans[j].x - fuelCans[i].x) < LANE_WIDTH/2 &&
                            abs(fuelCans[j].y - fuelCans[i].y) < 80) {
                            tooClose = true;
                            break;
                        }
                    }
                } while (tooClose);
                
                fuelCans[i].active = true;
            }
        }
    }
}

int findNearestFuelCan() {
    int nearestIndex = -1;
    float minDistance = 1000000;
    
    for(int i = 0; i < 36; i++) {
        if (fuelCans[i].active) {
            if (fuelCans[i].y > vehicleY) {
                float distance = sqrt(pow(fuelCans[i].x - vehicleX, 2) + pow(fuelCans[i].y - vehicleY, 2));
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestIndex = i;
                }
            }
        }
    }
    return nearestIndex;
}

void updateRandomMovement() {
    int nearestCan = findNearestFuelCan();
    
    if (nearestCan != -1) {
        int targetLane = (fuelCans[nearestCan].x - ROAD_LEFT) / LANE_WIDTH;
        
        if (currentLane != targetLane && rand() % 100 < 15) {
            if (currentLane < targetLane && currentLane < 2) {
                currentLane++;
                vehicleX = ROAD_LEFT + currentLane * LANE_WIDTH + LANE_WIDTH / 2;
                PlayLaneChangeSound();
            } else if (currentLane > targetLane && currentLane > 0) {
                currentLane--;
                vehicleX = ROAD_LEFT + currentLane * LANE_WIDTH + LANE_WIDTH / 2;
                PlayLaneChangeSound();
            }
        }
    } else {
        if (rand() % 100 < 5) {
            int newLane = currentLane;
            
            if (rand() % 2 == 0) {
                if (currentLane > 0) {
                    newLane = currentLane - 1;
                }
            } else {
                if (currentLane < 2) {
                    newLane = currentLane + 1;
                }
            }
            
            if (newLane != currentLane) {
                currentLane = newLane;
                vehicleX = ROAD_LEFT + currentLane * LANE_WIDTH + LANE_WIDTH / 2;
                PlayLaneChangeSound();
            }
        }
    }
}

void drawExplosion(int x, int y, int radius) {
    for(int r = radius; r > 0; r -= 10) {
        if(r % 20 == 0) {
            glColor3ub(255, 50, 0);
        } else {
            glColor3ub(255, 200, 0);
        }
        
        glBegin(GL_POLYGON);
        for(int i = 0; i < 32; i++) {
            double angle = (double)i * 6.28318530718 / 32.0;
            glVertex2d(x + cos(angle) * r, y + sin(angle) * r);
        }
        glEnd();
    }
}

void drawGameOver() {
    glColor3ub(255, 0, 0);
    const char* text = "GAME OVER!";
    int textWidth = strlen(text) * 20;
    glRasterPos2i(400 - textWidth/2, 300);
    YsGlDrawFontBitmap20x32(text);
    
    char scoreText[64];
    sprintf(scoreText, "Final Score: %d", score);
    int scoreWidth = strlen(scoreText) * 20;
    glRasterPos2i(400 - scoreWidth/2, 350);
    YsGlDrawFontBitmap16x24(scoreText);
    
    const char* exitText = "Press ESC to exit";
    int exitWidth = strlen(exitText) * 20;
    glRasterPos2i(400 - exitWidth/2, 400);
    YsGlDrawFontBitmap16x24(exitText);
}

int main(void) {
    FsOpenWindow(0, 0, 800, 600, 1);
    srand(time(0));

    soundPlayer.Start();
    LoadSounds();
    initScenery();

    vehicleX = ROAD_LEFT + currentLane * LANE_WIDTH + LANE_WIDTH / 2;
    initFuelCans();

    int frame = 0;
    bool running = true;
    int countdownTimer = 0;
    int countdownNumber = 3;
    bool showCountdown = false;
    bool gameStarted = false;
    bool showRace = false;
    int startTime = 0;
    int gameTimer = 0;
    score = initialFuel;
    lastScoreDeductionTime = time(0);

    introScreen = true;
    stickManX = 100;
    stickManY = 400;
    introCar = 600;
    titleAlpha = 0;
    introPhase = 0;

    while (running) {
        FsPollDevice();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (introScreen) {
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            if (introPhase == 0) {
                titleAlpha += 0.02;
                if (titleAlpha >= 1.0) {
                    introPhase = 1;
                }
            }

            glColor3ub(255, 0, 0);
            glRasterPos2i(150, 150);
            YsGlDrawFontBitmap20x32("NOT FAST AND NOT FURIOUS");

            if (introPhase >= 1) {
                drawStickMan(stickManX, stickManY);
                drawIntroCar(introCar, stickManY);

                if (stickManX < introCar - 50) {
                    stickManX += 2;
                } else {
                    showCountdown = true;
                    introPhase = 2;
                }
            }

            if (showCountdown) {
                countdownTimer++;
                if (countdownTimer > 30) {
                    countdownNumber--;
                    countdownTimer = 0;

                    if (countdownNumber == 0) {
                        showRace = true;
                    }
                }

                if (countdownNumber > 0) {
                    drawCountdownNumber(countdownNumber, 400, 300);
                } else if (showRace) {
                    drawRaceText(400, 300);
                    countdownTimer++;
                    if (countdownTimer > 30) {
                        introScreen = false;
                        gameStarted = true;
                    }
                }
            }
        } else {
            if (startTime == 0) {
                startTime = time(0);
                lastScoreDeductionTime = time(0);
            }
            gameTimer = time(0) - startTime;
            
            if (!isGameOver && gameStarted) {
                int currentTime = time(0);
                if (currentTime != lastScoreDeductionTime) {
                    score -= 100;
                    lastScoreDeductionTime = currentTime;
                }
            }
            
            if (score >= 3600 && !allFuelCollected) {
                allFuelCollected = true;
                for(int i = 0; i < 36; i++) {
                    fuelCans[i].active = false;
                }
            }
            
            if (score <= 0 && !isGameOver) {
                score = 0;
                isGameOver = true;
                isExploding = true;
                explosionRadius = 0;
                PlayCollisionSound();
            }
            
            drawRoad();
            drawTimer(gameTimer);
            drawScenery();
            drawDividers();
            
            if (!isGameOver) {
                drawVehicle();
                if (!allFuelCollected) {
                    updateAndDrawFuelCans();
                }
                
                movd -= 16;
                if (movd < -60) {
                    movd = 0;
                }
                
                if (frame % 10 == 0) {
                    updateRandomMovement();
                }

                glColor3ub(255, 255, 0);
                if (allFuelCollected) {
                    const char* msg = "All Fuel Collected! Conserve Remaining Fuel!";
                    glRasterPos2i(200, 100);
                    YsGlDrawFontBitmap16x24(msg);
                }
            } else {
                if (isExploding) {
                    drawExplosion(vehicleX, vehicleY, explosionRadius);
                    explosionRadius += 5;
                    if (explosionRadius > 100) {
                        isExploding = false;
                    }
                } else {
                    drawGameOver();
                }
            }
            
            glColor3ub(255, 255, 0);
            if (score < 500) {
                glColor3ub(255, 0, 0);
            }
            char scoreStr[64];
            sprintf(scoreStr, "Fuel: %d", score);
            glRasterPos2i(20, 30);
            YsGlDrawFontBitmap16x24(scoreStr);
            
            char timeStr[64];
            sprintf(timeStr, "Time: %d s", gameTimer);
            glRasterPos2i(600, 30);
            YsGlDrawFontBitmap16x24(timeStr);
            
            if (FSKEY_ESC == FsInkey()) {
                running = false;
            }
        }

        FsSwapBuffers();
        FsSleep(25);
        frame++;
    }

    soundPlayer.End();
    return 0;
}

