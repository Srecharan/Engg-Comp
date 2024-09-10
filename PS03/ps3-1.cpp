#include <iostream>
#include <cmath>

using namespace std;

// Constants for altitude and temperature
const int ALT_STEPS = 6;
const int TEMP_STEPS = 4;
const double ALT[] = {0.0, 2000.0, 4000.0, 6000.0, 8000.0, 10000.0};
const double TEMP[] = {-20.0, 0.0, 20.0, 40.0};

// Function to check valid input
bool validInput(double alt, double temp) {
    return (alt >= ALT[0] && alt <= ALT[ALT_STEPS - 1]) && (temp >= TEMP[0] && temp <= TEMP[TEMP_STEPS - 1]);
}

// Linear interpolation
double lerp(double x, double x1, double x2, double y1, double y2) {
    return y1 + ((x - x1) / (x2 - x1)) * (y2 - y1);
}

// Get climb rate from table
double getClimbRate(int aIdx, int tIdx, const double table[ALT_STEPS][TEMP_STEPS]) {
    return table[aIdx][tIdx];
}

// Calculate climb rate with bilinear interpolation
double calcClimbRate(double alt, double temp, const double table[ALT_STEPS][TEMP_STEPS]) {
    int aIdx = 0, tIdx = 0;

    // Find altitude index
    for (int i = 0; i < ALT_STEPS - 1; ++i) {
        if (alt >= ALT[i] && alt <= ALT[i + 1]) {
            aIdx = i;
            break;
        }
    }

    // Find temperature index
    for (int i = 0; i < TEMP_STEPS - 1; ++i) {
        if (temp >= TEMP[i] && temp <= TEMP[i + 1]) {
            tIdx = i;
            break;
        }
    }

    double altLow = ALT[aIdx];
    double altHigh = ALT[aIdx + 1];
    double tempLow = TEMP[tIdx];
    double tempHigh = TEMP[tIdx + 1];

    double rate11 = getClimbRate(aIdx, tIdx, table);
    double rate12 = getClimbRate(aIdx, tIdx + 1, table);
    double rate21 = getClimbRate(aIdx + 1, tIdx, table);
    double rate22 = getClimbRate(aIdx + 1, tIdx + 1, table);

    double tempInterpLow = lerp(temp, tempLow, tempHigh, rate11, rate12);
    double tempInterpHigh = lerp(temp, tempLow, tempHigh, rate21, rate22);

    return lerp(alt, altLow, altHigh, tempInterpLow, tempInterpHigh);
}

int main() {
    const double climbTable[ALT_STEPS][TEMP_STEPS] = {
        {830.0, 770.0, 705.0, 640.0},
        {720.0, 655.0, 595.0, 535.0},
        {645.0, 585.0, 525.0, 465.0},
        {530.0, 475.0, 415.0, 360.0},
        {420.0, 365.0, 310.0, 250.0},
        {310.0, 255.0, 200.0, 145.0}
    };

    double altitude, temperature;

    // Input loop
    while (true) {
        cout << "Enter altitude (0-10000 ft): ";
        cin >> altitude;
        cout << "Enter temperature (-20 to 40 °C): ";
        cin >> temperature;

        // Validate input
        if (!validInput(altitude, temperature)) {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        // Calculate and display climb rate
        double climbRate = calcClimbRate(altitude, temperature, climbTable);
        cout << "Estimated climb rate: " << climbRate << " ft/min.\n";
        break;
    }

    return 0;
}

