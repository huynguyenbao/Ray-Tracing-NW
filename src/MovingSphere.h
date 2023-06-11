#pragma once

#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "RTNW.h"
#include "Hittable.h"

class MovingSphere : public Hittable {
public:
	MovingSphere(Point3 c0, Point3 c1, float tm0, float tm1, float r, shared_ptr<Material> mat)
		: center0(c0), center1(c1), time0(tm0), time1(tm1), radius(r), materialPtr(mat) {}

	bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
		Point3 c = center(ray.time());

		Vec3 oc = ray.origin() - c;
		Point3 o = ray.origin();
		Vec3 d = ray.direction();

		float A = d.length2();
		float halfB = dot(oc, d);
		float C = oc.length2() - radius * radius;

		float delta = halfB * halfB - A * C;
		if (delta < 0)
			return false;
		else {
			float t = (-halfB - sqrtf(delta)) / A;
			if (t < tMin || t > tMax) {
				t = (-halfB + sqrtf(delta)) / A;
				if (t < tMin || t > tMax) return false;
			}

			Point3 intersectionPoint = ray.at(t);
			Vec3 outwardNormal = unitVector(intersectionPoint - c);

			hitRecord.p = intersectionPoint;
			hitRecord.t = t;
			setNormal(hitRecord, ray, outwardNormal);
			hitRecord.materialPtr = materialPtr;
			return true;
		}
	}

	bool boundingBox(float time0, float time1, AABB& bbox) const {
		AABB bbox0 = AABB(
			center(time0) - Vec3(radius, radius, radius),
			center(time0) + Vec3(radius, radius, radius));

		AABB bbox1 = AABB(
			center(time1) - Vec3(radius, radius, radius),
			center(time1) + Vec3(radius, radius, radius));

		bbox = surroundingBox(bbox0, bbox1);
		return true;
	}

	Point3 center(float time) const {
		return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
	}

private:
	Point3 center0, center1;
	float time0, time1;
	float radius;
	shared_ptr<Material> materialPtr;
};



#endif // !MOVING_SPHERE_H
