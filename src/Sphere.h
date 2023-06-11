#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "RTNW.h"
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

			getSphereUV(outwardNormal, hitRecord.u, hitRecord.v);
			return true;
		}
	}

	bool boundingBox(float time0, float time1, AABB& bbox) const {
		bbox = AABB(
			center - Vec3(radius, radius, radius),
			center + Vec3(radius, radius, radius));

		return true;
	}

	static void getSphereUV(const Point3& p, float& u, float& v) {
		// p: a given point on the sphere of radius one, centered at the origin.
		// u: returned value [0,1] of angle around the Y axis from X=-1.
		// v: returned value [0,1] of angle from Y=-1 to Y=+1.
		//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
		//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
		//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
		float theta = acosf(-p.y());
		float phi = atan2f(-p.z(), p.x()) + PI;

		u = phi / (2 * PI);
		v = theta / PI;
	}
public:
	Point3 center;
	float radius;
	shared_ptr<Material> materialPtr;
};

#endif // !SPHERE_H
