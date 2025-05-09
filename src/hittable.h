#pragma once

#include "aabb.h"

class Material;

struct HitRecord {
	Point3 p;
	Vec3 normal;
	std::shared_ptr<Material> mat;
	float t;
	float u;
	float v;
	bool frontFace;

	void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
		frontFace = dot(ray.dir, outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& ray, Interval rayT, HitRecord& record) const = 0;
	virtual AABB boundingBox() const = 0;
};