#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "RTNW.h"
#include "Ray.h"
#include "Hittable.h"
#include "Color.h"
#include "Texture.h"

class Material {
public:
	virtual bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatteredRay) const = 0;
	
	virtual Color emitted(float u, float v, const Point3& p) const {
		return Color(0, 0, 0);
	}
};

class Lambertian : public Material {
public:
	Lambertian(Color c) : albedo(make_shared<SolidColor>(c)) {}
	Lambertian(shared_ptr<Texture> texure) : albedo(texure) {}

	bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatter) const override {

		Vec3 reflectedVector = Vec3::randomInHemisphere(hitRecord.normal);
		//Vec3 reflectedVector = hitRecord.normal + randomUnitVector();
		if (reflectedVector.nearZero()) {
			reflectedVector = hitRecord.normal;
		}
		scatter = Ray(hitRecord.p, reflectedVector, ray_in.time());
		attenuation = albedo->value(hitRecord.u, hitRecord.v, hitRecord.p);

		return true;
	}
public:
	shared_ptr<Texture> albedo;
};

class Metal : public Material {
public:
	Metal(Color c, float f) : albedo(c), fuzzy(f < 1? f: 1) {}
	bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatter) const override {
		Vec3 reflectedVector = reflect(ray_in.direction(), hitRecord.normal) + fuzzy * Vec3::randomInUnitSphere();

		if (dot(reflectedVector, hitRecord.normal) < 0) return false;

		scatter = Ray(hitRecord.p, reflectedVector, ray_in.time());
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
		scatter = Ray(hitRecord.p, refractedVector, ray_in.time());
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

class DiffuseLight : public Material {
public:
	DiffuseLight(shared_ptr<Texture> texture) : emit(texture){}
	DiffuseLight(Color c) : emit(make_shared<SolidColor>(c)) {}

	virtual bool scatter(const Ray& ray_in, const HitRecord& hitRecord, Color& attenuation, Ray& scatteredRay) const override {
		return false;
	}

	virtual Color emitted(float u, float v, const Point3& p) const override {
		return emit->value(u, v, p);
	}
private:
	shared_ptr<Texture> emit;
};

class Isotropic : public Material {
public:
	Isotropic(Color a) : abedo(make_shared<SolidColor>(a)) {}
	Isotropic(shared_ptr<Texture> a) : abedo(a) {}

	virtual bool scatter(
		const Ray& ray_in, const HitRecord& hitRecord, 
		Color& attenuation, Ray& scatteredRay) const override 
	{
		scatteredRay = Ray(hitRecord.p, randomUnitVector(), ray_in.time());
		attenuation = abedo->value(hitRecord.u, hitRecord.v, hitRecord.p);
		return true;
	}
private:
	shared_ptr<Texture> abedo;
};

#endif // !MATERIAL_H
