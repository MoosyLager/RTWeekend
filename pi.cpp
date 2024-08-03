#include "rtweekend.h"

#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>

int main()
{
    std::cout << std::fixed << std::setprecision(12);

    int insideCircle = 0;
    int insideCircleStratified = 0;
    int sqrtN = 1000;

    for ( int i = 0; i < sqrtN; i++ ) {
        for ( int j = 0; j < sqrtN; j++ ) {
            auto x = RandomDouble(-1, 1);
            auto y = RandomDouble(-1, 1);
            if ( x * x + y * y < 1 ) insideCircle++;

            x = 2 * ((i + RandomDouble()) / sqrtN) - 1;
            y = 2 * ((j + RandomDouble()) / sqrtN) - 1;
            if ( x * x + y * y < 1 ) insideCircleStratified++;
        }
    }

    std::cout
        << "Regular    Estimate of Pi = "
        << (4.0 * insideCircle) / (sqrtN * sqrtN) << '\n'
        << "Stratified Estimate of Pi = "
        << (4.0 * insideCircleStratified) / (sqrtN * sqrtN) << '\n';
}