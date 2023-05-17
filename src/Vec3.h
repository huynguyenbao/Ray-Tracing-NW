#pragma once

#ifndef VEC3_H
#define VEC3_H

#include "RT1W.h"

#include <cmath>
#include <iostream>
#include <cassert>

using namespace std;

class Vec3 {
public:
	Vec3() : e{0, 0, 0}{}
	
	inline static Vec3 random() {
		return Vec3(random_float(), random_float(), random_float());
	}

	inline static Vec3 random(float min, float max) {
		return Vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}

	inline static Vec3 randomInUnitSphere() {
		while (true) {
			Vec3 v = Vec3::random(-1, 1);
			float len = v.length();
			if (len >= 1) continue;
			return v;
		}
	}
	
	inline static Vec3 randomInHemisphere(const Vec3& normal);

	Vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}
	
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }

	Vec3 operator -() { return Vec3(-e[0], -e[1], e[2]); }
	
	//float operator [] (int i) { 
	//	assert(i > -1 && i < 3);
	//	return e[i]; 
	//}
	
	float& operator [] (int i) { 
		assert(i > -1 && i < 3); 
		return e[i]; 
	}

	Vec3& operator += (const Vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vec3& operator *= (const Vec3& v) {
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}

	Vec3& operator *= (const float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vec3& operator /= (const float t) {
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}

	float length2() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	float length() const {
		return sqrtf(length2());
	}

	bool nearZero() const {
		float tolerance = 1e-8;
		return (fabs(e[0]) < tolerance) && (fabs(e[1]) <= tolerance) && (fabs(e[2]) <= tolerance);
	}

public:
	float e[3];
};


inline ostream& operator << (ostream& out, const Vec3& v) {
	return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline Vec3 operator + (const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3 operator - (const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator * (const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vec3 operator * (const float t, const Vec3& v) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator * (const Vec3& v, const float t) {
	return t * v;
}

inline Vec3 operator / (const Vec3& v, const float t) {
	return Vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float dot(const Vec3& v1, const Vec3& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(
		v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vec3 unitVector(const Vec3& v) {
	return v / v.length();
}

inline static Vec3 randomUnitVector() {
	return unitVector(Vec3::randomInUnitSphere());
}

inline Vec3 Vec3::randomInHemisphere(const Vec3& normal) {
	Vec3 v = randomInUnitSphere();
	if (dot(normal, v) > 0) return v;
	else return -1 * v;
}

inline Vec3 randomInUnitDisk() {
	while (true)
	{
		Vec3 p(random_float(-1, 1), random_float(-1, 1), 0);
		if (p.length() >= 1) continue;
		return p;
	}
	
	
}

inline static Vec3 reflect(const Vec3& incident, const Vec3& normal) {
	Vec3 i = incident;
	Vec3 n = unitVector(normal);
	Vec3 r = i + 2 * dot(-1 * i, n) * n;
	return r;
}

inline static Vec3 refract(const Vec3& incident, const Vec3& normal, float n1, float n2) {
	Vec3 i = unitVector(incident);
	Vec3 n = unitVector(normal);
	float cosTheta = dot(i * -1, n);
	
	Vec3 r_e = n1 / n2 * (i + cosTheta * n);
	Vec3 r_p = -sqrtf(1 - (n1 * n1) / (n2 * n2) * (1 - cosTheta * cosTheta)) * n;
	
	Vec3 r = r_p + r_e;
	return r;
}

typedef Vec3 Point3;

#endif // !VEC3_H
