#ifndef AARECT_H
#define AARECT_H

#include "RTNW.h"
#include "Hittable.h"

class XYRect : public Hittable {
public:
	XYRect() {};
	XYRect(float _x0, float _x1, float _y0, float _y1, float _k, shared_ptr<Material> mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat){}

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;

	virtual bool boundingBox(float time0, float time1, AABB& bbox) const override;

private:
	shared_ptr<Material> mat_ptr;
	float x0, x1, y0, y1, k;
};

bool XYRect::boundingBox(float time0, float time1, AABB& bbox) const {
	bbox = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
	return true;
}

bool XYRect::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	if (ray.direction().z() == 0) return false;
	float t = (k - ray.origin().z()) / ray.direction().z();
	if (t < tMin || t > tMax)
		return false;

	float x = ray.origin().x() + t * ray.direction().x();
	float y = ray.origin().y() + t * ray.direction().y();

	if (x < x0 || x > x1 || y < y0 || y > y1) 
		return false;

	hitRecord.u = (x - x0) / (x1 - x0);
	hitRecord.v = (y - y0) / (y1 - y0);
	hitRecord.t = t;
	hitRecord.materialPtr = mat_ptr;
	Vec3 outwardNormal = Vec3(0, 0, 1);
	setNormal(hitRecord, ray, outwardNormal);
	hitRecord.p = ray.at(t);

	if (Vec3::isNan(hitRecord.p)) 
		cout << "XYRect" << endl;
	return true;
}

// ----------------------------------

class YZRect : public Hittable {
public:
	YZRect() {};
	YZRect(float _y0, float _y1, float _z0, float _z1, float _k, shared_ptr<Material> mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;

	virtual bool boundingBox(float time0, float time1, AABB& bbox) const override;

private:
	shared_ptr<Material> mat_ptr;
	float y0, y1, z0, z1, k;
};

bool YZRect::boundingBox(float time0, float time1, AABB& bbox) const {
	bbox = AABB(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
	return true;
}

bool YZRect::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	if (ray.direction().x() == 0) return false;
	float t = (k - ray.origin().x()) / ray.direction().x();
	if (t < tMin || t > tMax)
		return false;

	float y = ray.origin().y() + t * ray.direction().y();
	float z = ray.origin().z() + t * ray.direction().z();

	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;

	hitRecord.u = (y - y0) / (y1 - y0);
	hitRecord.v = (z - z0) / (z1 - z0);
	hitRecord.t = t;
	hitRecord.materialPtr = mat_ptr;
	Vec3 outwardNormal = Vec3(1, 0, 0);
	setNormal(hitRecord, ray, outwardNormal);
	hitRecord.p = ray.at(t);
	if (Vec3::isNan(hitRecord.p))
		cout << "YZRect" << endl;
	return true;
}

// ----------------------------------

class XZRect : public Hittable {
public:
	XZRect() {};
	XZRect(float _x0, float _x1, float _z0, float _z1, float _k, shared_ptr<Material> mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;

	virtual bool boundingBox(float time0, float time1, AABB& bbox) const override;

private:
	shared_ptr<Material> mat_ptr;
	float x0, x1, z0, z1, k;
};

bool XZRect::boundingBox(float time0, float time1, AABB& bbox) const {
	bbox = AABB(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
	return true;
}

bool XZRect::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	if (ray.direction().y() == 0) return false;
	float t = (k - ray.origin().y()) / ray.direction().y();
	if (t < tMin || t > tMax)
		return false;

	float x = ray.origin().x() + t * ray.direction().x();
	float z = ray.origin().z() + t * ray.direction().z();

	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;

	hitRecord.u = (x - x0) / (x1 - x0);
	hitRecord.v = (z - z0) / (z1 - z0);
	hitRecord.t = t;
	hitRecord.materialPtr = mat_ptr;
	Vec3 outwardNormal = Vec3(0, 1, 0);
	setNormal(hitRecord, ray, outwardNormal);
	hitRecord.p = ray.at(t);
	if (Vec3::isNan(hitRecord.p))
		cout << "XZRect" << endl;
	return true;
}

#endif // !AARECT_H
