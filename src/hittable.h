#pragma once

#include "aabb.h"

class Material;

struct HitRecord 
{
	Point3 p;
	Vec3 normal;
	std::shared_ptr<Material> mat;
	float t;
	float u;
	float v;
	bool frontFace;

	void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) 
	{
		frontFace = dot(ray.dir, outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable 
{
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& ray, Interval rayT, HitRecord& record) const = 0;
	virtual AABB boundingBox() const = 0;
};

class Translate : public Hittable
{
private:
	std::shared_ptr<Hittable> object;
	Vec3 offset;
	AABB bbox;

public:
	Translate(std::shared_ptr<Hittable> object, const Vec3& offset)
		: object(object), offset(offset)
	{
		bbox = object->boundingBox() + offset;
	}

	bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
	{
		// Transform the ray from world space to object space
		Ray offsetRay(ray.origin - offset, ray.dir, ray.time);

		// Determine if and where an intersection exists in object space
		if (!object->hit(offsetRay, rayT, record))
			return false;

		// Transform the intersection point back to world space
		record.p += offset;
		return true;
	}

	AABB boundingBox() const override { return bbox; }
};

class RotateY : public Hittable
{
private:
	std::shared_ptr<Hittable> object;
	float sinTheta;
	float cosTheta;
	AABB bbox;

public:
	RotateY(std::shared_ptr<Hittable> object, float angle) : object(object)
	{
		float radians = degreesToRadians(angle);
		sinTheta = std::sin(radians);
		cosTheta = std::cos(radians);
		bbox = object->boundingBox();

		// Calculate the bounding box of the rotated object
		Point3 min(infinity, bbox.y.min, infinity);
		Point3 max(infinity, bbox.y.max, infinity);

		for (int i = 0; i < 2; i++)
		{
			for (int k = 0; k < 2; k++)
			{
				float cornerX = i * bbox.x.max + (1 - i) * bbox.x.min;
				float cornerZ = k * bbox.z.max + (1 - k) * bbox.z.min;

				float x = cosTheta * cornerX + sinTheta * cornerZ;
				float z = -sinTheta * cornerX + cosTheta * cornerZ;

				min.x = std::fmin(min.x, x);
				max.x = std::fmax(max.x, x);
				min.z = std::fmin(min.z, z);
				max.z = std::fmax(max.z, z);
			}
		}

		bbox = AABB(min, max);
	}

	bool hit(const Ray& r, Interval rayT, HitRecord& record) const override
	{
		// Transform the ray from world space to object space
		Point3 origin = rotateToObject(r.origin);
		Vec3 dir = rotateToObject(r.dir);
		Ray rotatedRay(origin, dir, r.time);

		// Determine if and where an intersection exists in object space
		if (!object->hit(rotatedRay, rayT, record))
			return false;

		// Transform the intersection point back to world space
		record.p = rotateToWorld(record.p);
		record.normal = rotateToWorld(record.normal);
		return true;
	}

	Vec3 rotateToObject(const Vec3& v) const
	{
		return Vec3(
			cosTheta * v.x - sinTheta * v.z,
			v.y,
			sinTheta * v.x + cosTheta * v.z
		);
	}

	Vec3 rotateToWorld(const Vec3& v) const
	{
		return Vec3(
			cosTheta * v.x + sinTheta * v.z,
			v.y,
			-sinTheta * v.x + cosTheta * v.z
		);
	}

	AABB boundingBox() const override { return bbox; }
};