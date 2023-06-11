#pragma once

#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "RTNW.h"
#include "HittableList.h"
#include <algorithm>

class BVHNode : public Hittable {
public:
	BVHNode() {}

	BVHNode(const HittableList& list, float time0, float time1)
		: BVHNode(list.objects, 0, list.objects.size(), time0, time1) {};

	BVHNode(vector<shared_ptr<Hittable>>& srcObjects, size_t start, size_t end, float time0, float time1);

	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;

	virtual bool boundingBox(float time0, float time1, AABB& outBBox) const override;

private:
	bool box_compare_x(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2);
	bool box_compare_y(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2);
	bool box_compare_z(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2);
	bool box_compare(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2, int axis);

	shared_ptr<Hittable> leftNode;
	shared_ptr<Hittable> rightNode;
	AABB bbox;
};

bool BVHNode::boundingBox(float time0, float time1, AABB& outBBox) const {
	outBBox = bbox;
	return true;
}

bool BVHNode::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
	if (!bbox.hit(ray, tMin, tMax)) return false;

	bool hitLeft = leftNode->hit(ray, tMin, tMax, hitRecord);
	// Need to check again
	bool hitRight = rightNode->hit(ray, tMin, hitLeft ? hitRecord.t : tMax, hitRecord);

	return hitLeft || hitRight;
}

BVHNode::BVHNode(vector<shared_ptr<Hittable>>& srcObjects, size_t start, size_t end, float time0, float time1) {
	auto objects = srcObjects;

	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_compare_x 
					: (axis == 1) ? box_compare_y 
								  : box_compare_z;

	size_t range = end - start;

	if (range == 1) {
		leftNode = rightNode = srcObjects[start];
	}
	else if (range == 2) {
		if (comparator(srcObjects[start], srcObjects[start + 1])) {
			leftNode = srcObjects[start];
			rightNode = srcObjects[start + 1];
		}
		else {
			leftNode = srcObjects[start + 1];
			rightNode = srcObjects[start];
		}
	}
	else {
		std::sort(srcObjects.begin() + start, srcObjects.begin() + end, comparator);

		auto mid = start + range / 2;
		leftNode = make_shared<BVHNode>(srcObjects, start, mid, time0, time1);
		rightNode = make_shared<BVHNode>(srcObjects, mid, end, time0, time1);
	}


	AABB leftBox, rightBox;

	if (!leftNode->boundingBox(time0, time1, leftBox) || !rightNode->boundingBox(time0, time1, rightBox) {
		cout << "NO bounding box is created in BVHNode constructor\n";
	}
	
	bbox = surroundingBox(leftBox, rightBox);
}

bool BVHNode::box_compare(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2, int axis) {
	AABB bbox1, bbox2;
	if (!obj1->boundingBox(0, 0, bbox1) || !obj2->boundingBox(0, 0, bbox2)) {
		cout << "NO bounding box is created in BVHNode constructor\n";
	}
	return bbox1.min()[axis] < bbox2.min()[axis];
}

bool BVHNode::box_compare_x(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2) {
	return box_compare(obj1, obj2, 0);
}

bool BVHNode::box_compare_y(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2) {
	return box_compare(obj1, obj2, 1);
}

bool BVHNode::box_compare_z(const shared_ptr<Hittable>& obj1, const shared_ptr<Hittable>& obj2) {
	return box_compare(obj1, obj2, 2);
}

#endif // !BVH_NODE_H
