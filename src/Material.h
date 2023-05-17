#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "RT1W.h"
#include "Ray.h"
#include "Hittable.h"
#include "Color.h"

class Material {
public:
	virtual bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatteredRay) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(Color c) : albedo(c) {}
	bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatter) const override {
		Vec3 reflectedVector = hitRecord.normal + Vec3::randomInHemisphere(hitRecord.normal);
		if (reflectedVector.nearZero()) {
			reflectedVector = hitRecord.normal;
		}
		scatter = Ray(hitRecord.p, reflectedVector);
		attenuation = albedo;
		
		return true;
	}
public:
	Color albedo;
};

class Metal : public Material {
public:
	Metal(Color c, float f) : albedo(c), fuzzy(f < 1? f: 1) {}
	bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatter) const override {
		Vec3 reflectedVector = reflect(ray_in.direction(), hitRecord.normal) + fuzzy * Vec3::randomInUnitSphere();

		if (dot(reflectedVector, hitRecord.normal) < 0) return false;

		scatter = Ray(hitRecord.p, reflectedVector);
		attenuation = albedo;

		return true;
	}
public:
	Color albedo;
	float fuzzy;
};


class Dielectric : public Material {
public:
	Dielectric(float indexOfRefraction) : ir(indexOfRefraction) {}
	bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatter) const override {
		Vec3 refractedVector;
		float n1, n2;
		if (hitRecord.frontFace) {
			n1 = 1; 
			n2 = ir;
		}
		else {
			n1 = ir;
			n2 = 1;
		}

		float cosTheta = dot(ray_in.direction() * -1, hitRecord.normal);
		float sinTheta = sqrtf(1 - cosTheta * cosTheta);
		
		bool can_not_refract = n1 / n2 * sinTheta > 1;
		if (can_not_refract || reflectanceCoef(cosTheta, n1, n2) > random_float()) {
			// Must Reflect
			refractedVector = reflect(ray_in.direction(), hitRecord.normal);
		}
		else {
			// Must Refract
			refractedVector = refract(ray_in.direction(), hitRecord.normal, n1, n2);
		}
		scatter = Ray(hitRecord.p, refractedVector);
		attenuation = Color(1, 1, 1);

		return true;
	}
public:
	float ir;

private:
	float reflectanceCoef(float cosTheta, float n1, float n2) const {
		// Use Schilick's Approximation
		float r0 = powf((n1 - n2) / (n1 + n2), 2);
		return r0 + (1 - r0) * (1 - powf(cosTheta, 5));
	}
};

#endif // !MATERIAL_H
