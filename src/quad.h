#pragma once

#include "hittable.h"
#include "hittable_list.h"

class Quad : public Hittable
{
private:
	Point3 q;
	Vec3 u, v;
	Vec3 w;
	std::shared_ptr<Material> mat;
	AABB bbox;
	Vec3 normal;
	float d;

public:
	Quad(const Point3& q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> mat)
		: q(q), u(u), v(v), mat(mat)
	{
		Vec3 n = cross(u, v);
		normal = normalize(n);
		d = dot(normal, q);
		w = n / dot(n, n);

		setBoundingBox();
	}

	virtual void setBoundingBox()
	{
		// Compute the bounding box of all four vertices
		auto bboxDiagonal1 = AABB(q, q + u + v);
		auto bboxDiagonal2 = AABB(q + u, q + v);
		bbox = AABB(bboxDiagonal1, bboxDiagonal2);
	}

	bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
	{
		// Check if ray is parallel to the plane
		float denominator = dot(normal, ray.dir);
		if (std::fabs(denominator) < 1e-8f) 
			return false;

		// Check if hit point is outside the ray interval
		float t = (d - dot(normal, ray.origin)) / denominator;
		if (!rayT.contains(t))
			return false;

		// Determine if the hit point lies within the planar shape using its plane coordinates
		Point3 intersection = ray.at(t);
		Point3 planarHitPoint = intersection - q;
		float alpha = dot(w, cross(planarHitPoint, v));
		float beta = dot(w, cross(u, planarHitPoint));
		if (!isInterior(alpha, beta, record))
			return false;

		// Set the rest of the hit record if the ray hits the quad
		record.t = t;
		record.p = intersection;
		record.mat = mat;
		record.setFaceNormal(ray, normal);

		return true;
	}

	virtual bool isInterior(float a, float b, HitRecord& record) const
	{
		Interval unitInterval = Interval(0.0f, 1.0f);
		if (!unitInterval.contains(a) || !unitInterval.contains(b))
			return false;

		record.u = a;
		record.v = b;
		return true;
	}

	AABB boundingBox() const override { return bbox; }
};

inline std::shared_ptr<HittableList> box(const Point3& a, const Point3& b, std::shared_ptr<Material> mat)
{
	// Returns the 3D box that contains two opposite vertices a and b
	auto sides = std::make_shared<HittableList>();

	auto min = Point3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
	auto max = Point3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));

	auto dx = Vec3(max.x - min.x, 0.0f, 0.0f);
	auto dy = Vec3(0.0f, max.y - min.y, 0.0f);
	auto dz = Vec3(0.0f, 0.0f, max.z - min.z);

	sides->add(std::make_shared<Quad>(Point3(min.x, min.y, max.z), dx, dy, mat)); // front
	sides->add(std::make_shared<Quad>(Point3(max.x, min.y, max.z), -dz, dy, mat)); // right
	sides->add(std::make_shared<Quad>(Point3(max.x, min.y, min.z), -dx, dy, mat)); // back
	sides->add(std::make_shared<Quad>(Point3(min.x, min.y, min.z), dz, dy, mat)); // left
	sides->add(std::make_shared<Quad>(Point3(min.x, max.y, max.z), dx, -dz, mat)); // top
	sides->add(std::make_shared<Quad>(Point3(min.x, min.y, min.z), dx, dz, mat)); // bottom

	return sides;
}