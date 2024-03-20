#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "aabb.h"
#include "hitable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HitableList : public Hitable
{
public:
    std::vector<shared_ptr<Hitable>> objects;

    HitableList() {}

    HitableList(shared_ptr<Hitable> object) { Add(object); }

    void Clear() { objects.clear(); }

    void Add(shared_ptr<Hitable> object)
    {
        objects.push_back(object);
        boundingBox = AABB(boundingBox, object->BoundingBox());
    }

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
        HitRecord tempRecord;
        bool hitAnything = false;
        auto closestSoFar = rayT.max;

        for ( const auto &object : objects ) {
            if ( object->Hit(ray, Interval(rayT.min, closestSoFar), tempRecord) ) {
                hitAnything = true;
                closestSoFar = tempRecord.t;
                record = tempRecord;
            }
        }

        return hitAnything;
    }

    AABB BoundingBox() const override { return boundingBox; }

private:
    AABB boundingBox;
};

#endif