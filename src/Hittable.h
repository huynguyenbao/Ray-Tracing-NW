#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "RT1W.h"
#include "Ray.h"

typedef struct HitRecord {
	Point3 p;
	Vec3 normal;
	float t;
	bool frontFace;
	shared_ptr<Material> materialPtr;
} HitRecord;

inline void setNormal(HitRecord& hitRecord, const Ray& ray, const Vec3& outwardNormal) {
	hitRecord.frontFace = dot(ray.direction(), outwardNormal) > 0 ? false : true;
	hitRecord.normal = hitRecord.frontFace ? outwardNormal : -1 * outwardNormal;
}

class Hittable {
public:
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const = 0;
};

#endif // !HITTABLE_H
