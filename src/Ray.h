#pragma once

#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
public:
	Ray() {};
	Ray(const Point3& o, const Vec3& d, float tm) : _origin(o), _direction(d), _time(tm) {}

	Point3 origin() const { return _origin; }
	Vec3 direction() const { return _direction; }
	float time() const { return _time; }

	Point3 at(float t) const {
		return _origin + _direction * t;
	}

private:
	Point3 _origin;
	Vec3 _direction;
	float _time;
};

#endif // !RAY_H

