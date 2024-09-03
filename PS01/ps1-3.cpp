#include <iostream>
#include <cmath>

int main() 
{
    double a, b, c, d, e, f; 
    std::cout << "ax+by=c" << std::endl;
    std::cout << "dx+ey=f" << std::endl;
    std::cout << "Enter a b c d e f: ";   // Getting values from the User
    std::cin >> a >> b >> c >> d >> e >> f;
    double det = a * e - b * d;
 
    if (std::abs(det) < 0.000001)         // Checking if the determinant is close to zero
    {
        std::cout << "No solution." << std::endl;
    } 
    else 
    {
        double x = (e * c - b * f)/det;
        double y = (a * f - c * d)/det;
        std::cout << "x=" << x << " y=" << y << std::endl;
    }
    return 0;
}

