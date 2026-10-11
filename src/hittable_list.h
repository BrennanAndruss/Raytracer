#pragma once

#include "aabb.h"
#include "hittable.h"

#include <vector>

class HittableList : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<Hittable> object)
	{
		objects.push_back(object);
		bbox = AABB(bbox, object->boundingBox());
	}

	bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
	{
		HitRecord tempRecord;
		bool hitAnything = false;
		float closest = rayT.max;

		for (const auto& object : objects)
		{
			if (object->hit(ray, Interval(rayT.min, closest), tempRecord))
			{
				hitAnything = true;
				closest = tempRecord.t;
				record = tempRecord;
			}
		}

		return hitAnything;
	}

	AABB boundingBox() const override { return bbox; }

	float pdfValue(const Point3& origin, const Vec3& direction) const override
	{
		float weight = 1.0f / objects.size();
		float sum = 0.0f;

		for (const auto& object : objects)
		{
			sum += weight * object->pdfValue(origin, direction);
		}

		return sum;
	}

	Vec3 random(const Point3& origin) const override
	{
		int size = static_cast<int>(objects.size());
		return objects[randomInt(0, size - 1)]->random(origin);
	}

private:
	AABB bbox;
};