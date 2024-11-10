#include <stdio.h>
#include <string.h>

// Converts bowling notation to numeric score
int getScore(char roll, int prevRoll) {
    if (roll == 'X' || roll == 'x') return 10;
    if (roll == '/') return 10 - prevRoll;
    if (roll == '-' || roll == ' ') return 0;
    if (roll >= '0' && roll <= '9') return roll - '0';
    return 0;
}

// Calculate score for a single frame
int frameScore(const int scores[], int frameIdx) {
    int baseScore;
    int startPos = frameIdx * 2;
    
    // Handle 10th frame differently
    if (frameIdx == 9) {
        return scores[18] + scores[19] + scores[20];
    }

    if (scores[startPos] == 10) {
        baseScore = 10;
        if (scores[startPos + 2] == 10) {
            baseScore += 10;
            baseScore += (frameIdx < 8) ? scores[startPos + 4] : scores[startPos + 3];
        } else {
            baseScore += scores[startPos + 2] + scores[startPos + 3];
        }
        return baseScore;
    }
    
    baseScore = scores[startPos] + scores[startPos + 1];
    if (baseScore == 10) {
        return 10 + scores[startPos + 2];
    }
    
    return baseScore;
}

int main(void) {
    char input[256];
    int bowlingScores[21] = {0};
    int totalScore = 0;
    int currentScore = 0;
    int idx = 0;
    
    fgets(input, sizeof(input), stdin);
    size_t len = strlen(input);
    if (input[len-1] == '\n') input[--len] = '\0';
    
    if (len < 21) {
        printf("Too Short");
        return 0;
    }
    
    for (int i = 0; input[i]; i++) {
        if (input[i] != 'X' && input[i] != 'x' && input[i] != '/' && 
            input[i] != '-' && input[i] != ' ' && 
            (input[i] < '0' || input[i] > '9')) {
            printf("Wrong Character");
            return 0;
        }
    }
    
    // Convert scores if all characters are valid
    for (int i = 0; input[i] && idx < 21; i++) {
        bowlingScores[idx] = getScore(input[i], currentScore);
        currentScore = (input[i] >= '0' && input[i] <= '9') ? bowlingScores[idx] : 
                      ((input[i] == 'X' || input[i] == 'x') ? 10 : 0);
        idx++;
    }
    
    // Calculate total score
    for (int frame = 0; frame < 10; frame++) {
        totalScore += frameScore(bowlingScores, frame);
    }
    
    printf("%d", totalScore);
    return 0;
}
