#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

void ShuffleArray(int N, int arr[]) 
{
    for (int i = N - 1; i > 0; --i) 
    {
        int j = rand() % (i + 1);
        swap(arr[i], arr[j]);
    }
}

void PrintFlashcard(int num1, int num2) {
    cout << "+-----+" << endl;
    cout << "|     |" << endl;
    cout << "|" << setw(2) << num1 << "x" << left << setw(2) << num2 << "|" << endl;
    cout << "|     |" << endl;
    cout << "+-----+" << endl;
}

int main() {
    srand(time(0));
    int numCards;

    do 
    {
        cout << "How many flash cards do you want to work on? (1-144): ";
        cin >> numCards;
        if (numCards < 1 || numCards > 144) 
        {
            cout << "The number of cards must be between 1 and 144." << endl;
        }
    } 
    while (numCards < 1 || numCards > 144);

    int flashcards[144];
    for (int i = 0; i < 144; ++i) {
        flashcards[i] = (i / 12 + 1) * 100 + (i % 12 + 1); // E.g., 1203 represents 12x3
    }

    ShuffleArray(144, flashcards);

    int correctAnswers = 0;
    time_t startTime = time(nullptr);  

    for (int i = 0; i < numCards; ++i) {
        int num1 = flashcards[i] / 100;  
        int num2 = flashcards[i] % 100;  
        int correctAnswer = num1 * num2;

        PrintFlashcard(num1, num2); 
        
        int userAnswer;
        cout << "Enter Your Answer> ";
        cin >> userAnswer;

        if (userAnswer == correctAnswer) {
            cout << "Correct!" << endl;
            correctAnswers++;
        } else {
            cout << "Incorrect! The correct answer is: " << correctAnswer << endl;
        }
        cout << endl;  
    }

    time_t endTime = time(nullptr);  
    int duration = static_cast<int>(difftime(endTime, startTime));  
    double percentage = (static_cast<double>(correctAnswers) / numCards) * 100;

    cout << "You answered " << numCards << " problems in " << duration << " seconds." << endl;
    cout << "You answered " << correctAnswers << " problems correctly ("
         << fixed << setprecision(2) << percentage << "%)." << endl;

    return 0;  
}

