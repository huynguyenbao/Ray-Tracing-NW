#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "RT1W.h"
#include "Hittable.h"
#include <vector>

using namespace std;

class HittableList : public Hittable {
public:
	HittableList (){}
	HittableList(shared_ptr<Hittable> object) { objects.push_back(object); }

	void add(shared_ptr<Hittable> object) { objects.push_back(object); }
	void clear() { objects.clear(); }

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;

private:
	vector<shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	HitRecord tmp;
	bool hitAnything = false;
	float tClosest = tMax;

	for (const shared_ptr<Hittable>& obj : objects) {
		if (obj->hit(ray, tMin, tClosest, tmp)) {
			hitAnything = true;
			tClosest = tmp.t;
			hitRecord = tmp;
		}
	}

	return hitAnything;
}

#endif // !HITTABLE_LIST_H
