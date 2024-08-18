#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "rtweekend.h"

#include "hitable.h"
#include "hitableList.h"

class BVHNode : public Hitable
{
private:
    shared_ptr<Hitable> left;
    shared_ptr<Hitable> right;
    AABB boundingBox;

    static bool BoxCompare(
        const shared_ptr<Hitable> a, const shared_ptr<Hitable> b, int axisIndex)
    {
        return a->BoundingBox().Axis(axisIndex).min < b->BoundingBox().Axis(axisIndex).min;
    }

    static bool BoxXCompare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b)
    {
        return BoxCompare(a, b, 0);
    }

    static bool BoxYCompare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b)
    {
        return BoxCompare(a, b, 1);
    }

    static bool BoxZCompare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b)
    {
        return BoxCompare(a, b, 2);
    }

public:
    BVHNode(const HitableList &list) : BVHNode(list.objects, 0, list.objects.size()) {}

    BVHNode(const std::vector<shared_ptr<Hitable>> &sourceObjects, size_t start, size_t end)
    {
        auto objects = sourceObjects; // Create a modifiable array of the source scene objects
        int axis = RandomInt(0, 2);
        auto Comparator = (axis == 0)   ? BoxXCompare
                          : (axis == 1) ? BoxYCompare
                                        : BoxZCompare;

        size_t objectSpan = end - start;

        if ( objectSpan == 1 ) {
            left = right = objects[start];
        } else if ( objectSpan == 2 ) {
            if ( Comparator(objects[start], objects[start + 1]) ) {
                left = objects[start];
                right = objects[start + 1];
            } else {
                left = objects[start + 1];
                right = objects[start];
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, Comparator);

            auto mid = start + objectSpan / 2;
            left = make_shared<BVHNode>(objects, start, mid);
            right = make_shared<BVHNode>(objects, mid, end);
        }

        boundingBox = AABB(left->BoundingBox(), right->BoundingBox());
    }

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
        if ( !boundingBox.Hit(ray, rayT) ) return false;

        bool hitLeft = left->Hit(ray, rayT, record);
        bool hitRight = right->Hit(ray, Interval(rayT.min, hitLeft ? record.t : rayT.max), record);

        return hitLeft || hitRight;
    }

    AABB BoundingBox() const override { return boundingBox; }

    double PDFValue(const Point3 &origin, const Vec3 &direction) const override
    {
        return 0.0;
    }

    Vec3 Random(const Point3 &origin) const override
    {
        return Vec3(1, 0, 0);
    }
};

#endif