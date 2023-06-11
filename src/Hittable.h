#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "RTNW.h"
#include "Ray.h"
#include "AABB.h"

typedef struct HitRecord {
	Point3 p; // intersection point
	Vec3 normal; // normal vector at p
	float t; // paramter of the ray's paramteric equation 
	bool frontFace; // ray hits the surface at a front face or back face?
	shared_ptr<Material> materialPtr; // material at the intersection point
	float u; // Texture Coordinate
	float v; // Texturee Coordinate
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

	virtual bool boundingBox(float time0, float time1, AABB& bbox) const = 0;
};

// ----------------------------------------

class Translate : public Hittable {
public:
	Translate(shared_ptr<Hittable> ptr, Vec3 t) : obj(ptr), offset(t) {}

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;
	virtual bool boundingBox(float time0, float time1, AABB& bbox) const override;
private:
	shared_ptr<Hittable> obj;
	Vec3 offset;
};

bool Translate::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	Ray translated_ray = Ray(ray.origin() - offset, ray.direction(), ray.time());

	if (!obj->hit(translated_ray, tMin, tMax, hitRecord)) {
		return false;
	}

	hitRecord.p += offset;
	setNormal(hitRecord, translated_ray, hitRecord.normal);
}

bool Translate::boundingBox(float time0, float time1, AABB& bbox) const {
	if (!obj->boundingBox(time0, time1, bbox)) {
		return false;
	}
	
	bbox = AABB(
		bbox.min() + offset,
		bbox.max() + offset
	);
	return true;
}

// ----------------------------------------

class RotateY : public Hittable {
public:
	RotateY(shared_ptr<Hittable> ptr, float angle);

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;
	virtual bool boundingBox(float time0, float time1, AABB& bbox) const override;
private:
	shared_ptr<Hittable> obj;
	float cos_theta, sin_theta;
	bool hasBox;
	AABB bbox;
};

RotateY::RotateY(shared_ptr<Hittable> ptr, float angle) : obj (ptr) {
	float radians = degrees2radian(angle);
	cos_theta = cosf(radians);
	sin_theta = sinf(radians);

	hasBox = ptr->boundingBox(0, 1, bbox);

	Point3 pMin(INF, INF, INF);
	Point3 pMax(-INF, -INF, -INF);

	for (int ii = 0; ii < 2; ii++) {
		for (int jj = 0; jj < 2; jj++) {
			for (int kk = 0; kk < 2; kk++) {
				float x = ii * bbox.min().x() + (1 - ii) * bbox.max().x();
				float y = jj * bbox.min().y() + (1 - jj) * bbox.max().y();
				float z = kk * bbox.min().z() + (1 - kk) * bbox.max().z();

				float xNew = cos_theta * x + sin_theta * z;
				float zNew = -sin_theta * x + cos_theta * z;

				Vec3 tmp = Vec3(xNew, y, zNew);
				for (int c = 0; c < 3; c++) {
					pMin[c] = fmin(pMin[c], tmp[c]);
					pMax[c] = fmax(pMax[c], tmp[c]);
				}
			}
		}
	}

	bbox = AABB(pMin, pMax);
}

bool RotateY::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	Point3 origin = ray.origin();
	Vec3 direction = ray.direction();

	// Inverse Rotation
	origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
	origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];

	direction[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
	direction[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];

	Ray rotated_ray = Ray(origin, direction, ray.time());

	if (!obj->hit(rotated_ray, tMin, tMax, hitRecord))
		return false;

	Point3 p = hitRecord.p;
	Vec3 normal = hitRecord.normal;

	// Rotation
	p[0] = cos_theta * hitRecord.p[0] + sin_theta * hitRecord.p[2];
	p[2] = -sin_theta * hitRecord.p[0] + cos_theta * hitRecord.p[2];

	normal[0] = cos_theta * hitRecord.normal[0] + sin_theta * hitRecord.normal[2];
	normal[2] = -sin_theta * hitRecord.normal[0] + cos_theta * hitRecord.normal[2];

	hitRecord.p = p;
	setNormal(hitRecord, rotated_ray, normal);

	return true;
}

bool RotateY::boundingBox(float time0, float time1, AABB& o_bbox) const {
	if (!hasBox) return false;
	
	o_bbox = bbox;
	return true;
}

#endif // !HITTABLE_H
