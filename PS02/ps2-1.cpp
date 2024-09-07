#include <iostream>
#include <vector>
#include <algorithm> 
#include <random>    
#include <ctime>     

int main() 
{
    std::vector<std::string> lunch = 
    {
        "Pizza from Mineo's",
        "Tofu Tikka Masala from Prince of India",
        "Crispy Salmon Roll from Sushi Fuku",
        "Sub from Uncle Sam's",
        "Fried rice from How Lee",
        "Sandwiches from La Prima",
        "Find free food on campus"
    };

    const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};  // Days

    std::random_device rd;                                                   // Creating a random number generator and shuffling the lunch options
    std::default_random_engine rng(rd());
    std::shuffle(lunch.begin(), lunch.end(), rng);
    
    for (int i = 0; i < 7; ++i)                                             // Print the lunch plan for the week
    {
        std::cout << days[i] << " " << lunch[i] << std::endl;
    }

    return 0;
}

