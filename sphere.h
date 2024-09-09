#pragma once

#include "hittable.h"

class Sphere : public Hittable {
private:
	Point3 center;
	float radius;

public:
	Sphere(const Point3& center, double radius)
		: center(center), radius(std::fmax(0.0f, radius)) {}

	bool hit(const Ray& ray, float rayTMin, float rayTMax, HitRecord& record) const override {
		Vec3 oc = center - ray.origin;
		float a = sqrMag(ray.dir);
		float h = dot(ray.dir, oc);
		float c = sqrMag(oc) - radius * radius;
		
		float discriminant = h * h - a * c;
		if (discriminant < 0) {
			return false;
		}
		
		float sqrtf = std::sqrt(discriminant);

		// Find the nearest root in the acceptable range
		float root = (h - sqrtf) / a;
		if (root <= rayTMin || rayTMax <= root) {
			root = (h + sqrtf) / a;
			if (root <= rayTMin || rayTMax <= root) {
				return false;
			}
		}

		record.t = root;
		record.p = ray.at(record.t);
		record.normal = (record.p - center) / radius;

		return true;
	}
};