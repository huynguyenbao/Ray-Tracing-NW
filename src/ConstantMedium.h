#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "RTNW.h"
#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium : public Hittable {
public:
	ConstantMedium(shared_ptr<Hittable> obj, float density, shared_ptr<Material> mat)
		: boundary(obj), negInvDensity(-1/density), phase_function(mat) {}
	
	ConstantMedium(shared_ptr<Hittable> obj, float density, Color c)
		: boundary(obj), negInvDensity(-1 / density), phase_function(make_shared<Isotropic>(c)) {}

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;

	virtual bool boundingBox(float time0, float time1, AABB& bbox) const override {
		return boundary->boundingBox(time0, time1, bbox);
	}
private:
	shared_ptr<Hittable> boundary;
	shared_ptr<Material> phase_function;
	float negInvDensity;
};

bool ConstantMedium::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	// Print occasional samples when debugging. To enable, set enableDebug true.
	const bool enableDebug = false;
	const bool debugging = enableDebug && random_float() < 0.0001;

	HitRecord rec1, rec2;

	if (!boundary->hit(ray, -INF, INF, rec1)) {
		return false;
	}
	if (!boundary->hit(ray, rec1.t + 0.0001, INF, rec2)) {
		return false;
	}
	
	if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';
	
	if (rec1.t < tMin) rec1.t = tMin;
	if (rec2.t > tMax) rec2.t = tMax;

	if (rec1.t >= rec2.t)
		return false;

	if (rec1.t < 0)
		rec1.t = 0;

	const auto rayLength = ray.direction().length();
	const auto distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
	const auto hitDistance = negInvDensity * log(random_float());

	if (hitDistance > distanceInsideBoundary) {
		return false;
		//Ray secondRay = Ray(ray.at(rec2.t + 0.0001), ray.direction(), ray.time());

		//if(!hit(secondRay, tMin, tMax, hitRecord))
		//	return false;

		//return true;
	}
	hitRecord.t = rec1.t + hitDistance / rayLength;
	hitRecord.p = ray.at(hitRecord.t);

	if (debugging) {
		std::cerr << "hitDistance = " << hitDistance << '\n'
			<< "hitRecord.t = " << hitRecord.t << '\n'
			<< "hitRecord.p = " << hitRecord.p << '\n';
	}

	hitRecord.normal = Vec3(1, 0, 0);  // arbitrary
	hitRecord.frontFace = true;     // also arbitrary
	hitRecord.materialPtr = phase_function;

	return true;
}

#endif // !CONSTANT_MEDIUM_H
