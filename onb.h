#ifndef ONB_H
#define ONB_H

#include "rtweekend.h"

class ONB
{
public:
    ONB(const Vec3 &n)
    {
        axis[2] = UnitVector(n);
        Vec3 a = (std::fabs(axis[2].X()) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
        axis[1] = UnitVector(Cross(axis[2], a));
        axis[0] = Cross(axis[2], axis[1]);
    }

    const Vec3 &U() const { return axis[0]; }

    const Vec3 &V() const { return axis[1]; }

    const Vec3 &W() const { return axis[2]; }

    Vec3 Transform(const Vec3 &v) const
    {
        // Transform from basis coordinates to local coordinates
        return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
    }

private:
    Vec3 axis[3];
};

#endif