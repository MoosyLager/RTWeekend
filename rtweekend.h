#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

// Usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double maxDouble = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility Functions
inline double DegreesTooRadians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double RandomDouble()
{
    static thread_local std::mt19937 generator;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double RandomDouble(double min, double max)
{
    // Returns a random real in [min,max)
    return min + (max - min) * RandomDouble();
}

inline int RandomInt(int min, int max)
{
    // Return a random integer in [min,max]
    return static_cast<int>(RandomDouble(min, max + 1));
}

// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif