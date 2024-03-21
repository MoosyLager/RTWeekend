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

    static double TrilinearInterpolation(double c[2][2][2], double u, double v, double w)
    {
        auto accumulation = 0.0;
        for ( int i = 0; i < 2; i++ ) {
            for ( int j = 0; j < 2; j++ ) {
                for ( int k = 0; k < 2; k++ ) {
                    accumulation += (i * u + (1 - i) * (1 - u)) *
                                    (j * v + (1 - j) * (1 - v)) *
                                    (k * w + (1 - k) * (1 - w)) * c[i][j][k];
                }
            }
        }
        return accumulation;
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
        auto u = p.X() - floor(p.X());
        auto v = p.Y() - floor(p.Y());
        auto w = p.Z() - floor(p.Z());
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);

        auto i = static_cast<int>(floor(p.X()));
        auto j = static_cast<int>(floor(p.Y()));
        auto k = static_cast<int>(floor(p.Z()));
        double c[2][2][2];

        for ( int di = 0; di < 2; di++ ) {
            for ( int dj = 0; dj < 2; dj++ ) {
                for ( int dk = 0; dk < 2; dk++ ) {
                    c[di][dj][dk] = randomFloat[permutationX[(i + di) & 255] ^
                                                permutationY[(j + dj) & 255] ^
                                                permutationZ[(k + dk) & 255]];
                }
            }
        }
        return TrilinearInterpolation(c, u, v, w);
    }
};

#endif