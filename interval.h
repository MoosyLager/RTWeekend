#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
public:
    double min, max;

    Interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    Interval(double _min, double _max) : min(_min), max(_max) {}

    Interval(const Interval &a, const Interval &b)
        : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

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

    double Size() const
    {
        return max - min;
    }

    Interval Expand(double delta) const
    {
        auto padding = delta / 2;
        return Interval(min - padding, max + padding);
    }

    static const Interval EMPTY, UNIVERSE;
};

const static Interval EMPTY(+infinity, -infinity);
const static Interval UNIVERSE(-infinity, +infinity);

Interval operator+(const Interval &ival, double displacement)
{
    return Interval(ival.min + displacement, ival.max + displacement);
}

Interval operator+(double displacement, const Interval &ival)
{
    return ival + displacement;
}

#endif