#include "rtweekend.h"

#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>

int main()
{
    int N = 100000;
    int insideCircle = 0;
    for ( int i = 0; i < N; i++ ) {
        auto x = RandomDouble(-1, 1);
        auto y = RandomDouble(-1, 1);
        if ( x * x + y * y < 1 ) insideCircle++;
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Estimate of PI = " << (4.0 * insideCircle) / N << "\n";
}