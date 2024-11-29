#pragma once

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class BVHNode : public Hittable {
public:
	BVHNode(HittableList& list) : BVHNode(list.objects, 0, list.objects.size()) {}

	BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
		bbox = AABB::Empty;
		for (size_t i = start; i < end; i++) {
			bbox = AABB(bbox, objects[i]->boundingBox());
		}

		int axis = bbox.longestAxis();

		auto comparator = [&axis](const auto& a, const auto& b) {
			return boxCompare(a, b, axis);
			};

		size_t objectSpan = end - start;

		if (objectSpan == 1) {
			left = right = objects[start];
		}
		else if (objectSpan == 2) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			auto mid = start + objectSpan / 2;
			left = std::make_shared<BVHNode>(objects, start, mid);
			right = std::make_shared<BVHNode>(objects, mid, end);
		}
	}

	bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override {
		if (!bbox.hit(ray, rayT)) return false;
		
		bool hitLeft = left->hit(ray, rayT, record);
		bool hitRight = right->hit(ray, Interval(rayT.min, hitLeft ? record.t : rayT.max), record);
		
		return hitLeft || hitRight;
	}

	AABB boundingBox() const override { return bbox; }

private:
	std::shared_ptr<Hittable> left;
	std::shared_ptr<Hittable> right;
	AABB bbox;

	static bool boxCompare(
		const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, int axis
	) {
		auto aInterval = a->boundingBox().axisInterval(axis);
		auto bInterval = b->boundingBox().axisInterval(axis);
		return aInterval.min < bInterval.min;
	}

	static bool boxCompareX(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
		return boxCompare(a, b, 0);
	}

	static bool boxCompareY(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
		return boxCompare(a, b, 1);
	}

	static bool boxCompareZ(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
		return boxCompare(a, b, 2);
	}
};