#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
public:
    double min, max;

    Interval() : min(+INF), max(-INF) {} // Default interval is empty

    Interval(double _min, double _max) : min(_min), max(_max) {}

    bool Contains(double x) const
    {
        return min <= x && x <= max;
    }

    bool Surrounds(double x) const
    {
        return min < x && x < max;
    }

    double Clamp(double x) const
    {
        if ( x < min ) return min;
        if ( x > max ) return max;
        return x;
    }

    static const Interval EMPTY, UNIVERSE;
};

const static Interval EMPTY(+INF, -INF);
const static Interval UNIVERSE(-INF, +INF);

#endif