#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "RT1W.h"
#include "Ray.h"

typedef struct HitRecord {
	Point3 p; // intersection point
	Vec3 normal; // normal vector at p
	float t; // paramter of the ray's paramteric equation 
	bool frontFace; // ray hits the surface at a front face or back face?
	shared_ptr<Material> materialPtr; // material at the intersection point
} HitRecord;

inline void setNormal(HitRecord& hitRecord, const Ray& ray, const Vec3& outwardNormal) {
	hitRecord.frontFace = dot(ray.direction(), outwardNormal) > 0 ? false : true;
	hitRecord.normal = hitRecord.frontFace ? outwardNormal : -1 * outwardNormal;
}

class Hittable {
public:
	// Find the closest intersection between a ray and surface and write to hitRecord, 
	// eliminate the intersection if it's out of [tMin, tMax]
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const = 0;
};

#endif // !HITTABLE_H
