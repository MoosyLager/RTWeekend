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
const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility Functions
inline double DegreesTooRadians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double RandomDouble()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double RandomDouble(double min, double max)
{
    // Returns a random real in [min,max)
    return min + (max - min) * RandomDouble();
}

// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif