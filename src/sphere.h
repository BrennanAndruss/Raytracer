#pragma once

#include "hittable.h"

class Sphere : public Hittable {
private:
	Ray center;
	float radius;
	std::shared_ptr<Material> mat;
	AABB bbox;

	static void getSphereUV(const Point3& p, float& u, float& v)
	{
		float theta = std::acos(-p.y);
		float phi = std::atan2(-p.z, p.x) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}

	static Vec3 randomToSphere(float radius, float distSquared)
	{
		auto r1 = randomFloat();
		auto r2 = randomFloat();
		float z = 1.0f + r2 * (std::sqrt(1.0f - radius * radius / distSquared) - 1.0f);

		float phi = 2.0f * pi * r1;
		float x = std::cos(phi) * std::sqrt(1.0f - z * z);
		float y = std::sin(phi) * std::sqrt(1.0f - z * z);

		return Vec3(x, y, z);
	}

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
		getSphereUV(outwardNormal, record.u, record.v);
		record.mat = mat;

		return true;
	}

	AABB boundingBox() const override { return bbox; }

	float pdfValue(const Point3& origin, const Vec3& direction) const override
	{
		HitRecord rec;
		if (!hit(Ray(origin, direction), Interval(0.001f, infinity), rec))
			return 0;

		// Sample the center at time 0
		// This method only works for stationary spheres
		float distSquared = sqrMag(center.at(0.0f) - origin);
		float cosThetaMax = std::sqrt(1.0f - radius * radius / distSquared);
		float solidAngle = 2.0f * pi * (1.0f - cosThetaMax);

		return 1.0f / solidAngle;
	}

	Vec3 random(const Point3& origin) const override
	{
		Vec3 direction = center.at(0.0f) - origin;
		float distSquared = sqrMag(direction);
		OrthonormalBasis uvw(direction);
		return uvw.transform(randomToSphere(radius, distSquared));
	}
};