#pragma once

#include "hittable.h"

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