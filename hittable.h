#pragma once

#include "ray.h"

struct HitRecord {
	Point3 p;
	Vec3 normal;
	float t;
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& ray, float rayTMin, float rayTMax, HitRecord& record) const = 0;
};