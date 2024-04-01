#include "rtweekend.h"

#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>

int main()
{
    int N = 100000;
    int runs = 0;
    int insideCircle = 0;
    std::cout << std::fixed << std::setprecision(12);
    while ( true ) {
        runs++;
        auto x = RandomDouble(-1, 1);
        auto y = RandomDouble(-1, 1);
        if ( x * x + y * y < 1 ) insideCircle++;

        if ( runs % 100000 == 0 ) {
            std::cout << "Estimate of PI = " << (4.0 * insideCircle) / runs << "\n";
        }
    }
}