#ifndef BOX_H
#define BOX_H

#include "RTNW.h"
#include "HittableList.h"
#include "AARect.h"

class Box : public Hittable {
public:
	Box() {}
	Box(const Point3& p0, const Point3& p1, shared_ptr<Material> mat);

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;
	virtual bool boundingBox(float time0, float time1, AABB& bbox) const override;

private:
	Point3 pMin, pMax;
	HittableList sides;
};

Box::Box(const Point3& p0, const Point3& p1, shared_ptr<Material> mat) {
	pMin = p0;
	pMax = p1;

	sides.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));
	sides.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));

	sides.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
	sides.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));

	sides.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));
	sides.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
}

bool Box::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	return sides.hit(ray, tMin, tMax, hitRecord);
}

bool Box::boundingBox(float time0, float time1, AABB& bbox) const {
	bbox = AABB(pMin, pMax);
	return true;
}

#endif // !BOX_H
