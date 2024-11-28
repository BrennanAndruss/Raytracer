#pragma once

#include "hittable.h"

class Sphere : public Hittable {
private:
	Ray center;
	float radius;
	std::shared_ptr<Material> mat;
	AABB bbox;

public:
	// Stationary Sphere
	Sphere(const Point3& staticCenter, float radius, std::shared_ptr<Material> mat)
		: center(staticCenter, Vec3()), radius(std::fmax(0.0f, radius)), mat(mat) 
	{
		auto rVec = Vec3(radius, radius, radius);
		bbox = AABB(staticCenter - rVec, staticCenter + rVec);
	}

	// Moving Sphere
	Sphere(const Point3& center1, const Point3& center2, float radius, std::shared_ptr<Material> mat)
		: center(center1, center2 - center1), radius(std::fmax(0.0f, radius)), mat(mat) 
	{
		auto rVec = Vec3(radius, radius, radius);
		AABB box1 = AABB(center1 - rVec, center1 + rVec);
		AABB box2 = AABB(center2 - rVec, center2 + rVec);
		bbox = AABB(box1, box2);
	}

	bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override {
		Vec3 currentCenter = center.at(ray.time);
		Vec3 oc = currentCenter - ray.origin;
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
		if (!rayT.surrounds(root)) {
			root = (h + sqrtf) / a;
			if (!rayT.surrounds(root)) {
				return false;
			}
		}

		record.t = root;
		record.p = ray.at(record.t);

		// Get the outward unit normal
		Vec3 outwardNormal = (record.p - currentCenter) / radius;
		record.setFaceNormal(ray, outwardNormal);
		record.mat = mat;

		return true;
	}

	AABB boundingBox() const override { return bbox; }
};