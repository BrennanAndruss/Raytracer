#pragma once

#include "hittable.h"
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<Hittable> object) {
		objects.push_back(object);
	}

	bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override {
		HitRecord tempRecord;
		bool hitAnything = false;
		float closest = rayT.max;

		for (const auto& object : objects) {
			if (object->hit(ray, Interval(rayT.min, closest), tempRecord)) {
				hitAnything = true;
				closest = tempRecord.t;
				record = tempRecord;
			}
		}

		return hitAnything;
	}
};