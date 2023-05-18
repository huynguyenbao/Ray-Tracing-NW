#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "RT1W.h"
#include "Vec3.h"
#include "Hittable.h"
#include "Ray.h"

class Sphere : public Hittable{
public:
	Sphere(){}
	Sphere(Point3 c, float r, shared_ptr<Material> material) : center(c), radius(r), materialPtr(material) {};

	// Solve a quaratic equation and save the result to hitRecord
	// Return true if the intersection point is valid, in the range [tMin, tMax], and false otherwise
	bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
		Vec3 oc = ray.origin() - center;
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
			Vec3 outwardNormal = unitVector(intersectionPoint - center);
			
			hitRecord.p = intersectionPoint;
			hitRecord.t = t;
			setNormal(hitRecord, ray, outwardNormal);
			hitRecord.materialPtr = materialPtr;
			return true;
		}
	}
public:
	Point3 center;
	float radius;
	shared_ptr<Material> materialPtr;
};

#endif // !SPHERE_H
