#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

class Perlin
{
private:
    static const int pointCount = 256;
    double* randomFloat;
    int* permutationX;
    int* permutationY;
    int* permutationZ;

    static int* PerlinGeneratePermutation()
    {
        auto p = new int[pointCount];

        for ( int i = 0; i < Perlin::pointCount; i++ )
            p[i] = i;

        Permute(p, pointCount);

        return p;
    }

    static void Permute(int* p, int n)
    {
        for ( int i = n - 1; i > 0; i-- ) {
            int target = RandomInt(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

public:
    Perlin()
    {
        randomFloat = new double[pointCount];
        for ( int i = 0; i < pointCount; i++ ) {
            randomFloat[i] = RandomDouble();
        }

        permutationX = PerlinGeneratePermutation();
        permutationY = PerlinGeneratePermutation();
        permutationZ = PerlinGeneratePermutation();
    }

    ~Perlin()
    {
        delete[] randomFloat;
        delete[] permutationX;
        delete[] permutationY;
        delete[] permutationZ;
    }

    double Noise(const Point3& p) const
    {
        auto i = static_cast<int>(4 * p.X()) & 255;
        auto j = static_cast<int>(4 * p.Y()) & 255;
        auto k = static_cast<int>(4 * p.Z()) & 255;

        return randomFloat[permutationX[i] ^ permutationY[j] ^ permutationZ[k]];
    }
};

#endif