#pragma once

#ifndef AABB_H
#define AABB_H

#include "RTNW.h"
#include "Ray.h"
#include "Vec3.h"

class AABB{
public:
	AABB(){}
	AABB(const Point3& minimum, const Point3& maximum) : pMin(minimum), pMax(maximum) {}

	Point3 min() const { return pMin; }
	Point3 max() const { return pMax; }

	bool hit(const Ray& ray, float tMin, float tMax) const {
		Point3 o = ray.origin();
		Vec3 d = ray.direction();

		for (int ii = 0; ii < 3; ii++) {
			float invD = 1 / d[ii];
			float t0 = (pMin[ii] - o[ii]) * invD;
			float t1 = (pMin[ii] - o[ii]) * invD;
			
			if (invD < 0) {
				float tmp = t0;
				t0 = t1;
				t1 = tmp;
			}

			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;

			if (tMax <= tMin) return false;
		}
		return true;
	}
private:
	Point3 pMin;
	Point3 pMax;
};

inline AABB surroundingBox(const AABB& bbox1, const AABB& bbox2) {
	Point3 pMin = Point3(
		fmin(bbox1.min().x(), bbox2.min().x()),
		fmin(bbox1.min().y(), bbox2.min().y()),
		fmin(bbox1.min().z(), bbox2.min().z())
		);

	Point3 pMax = Point3(
		fmin(bbox1.max().x(), bbox2.max().x()),
		fmin(bbox1.max().y(), bbox2.max().y()),
		fmin(bbox1.max().z(), bbox2.max().z())
		);

	return AABB(pMin, pMax);

}

#endif // !AABB_H
