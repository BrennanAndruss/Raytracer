#pragma once

#include "hittable.h"
#include "material.h"
#include "texture.h"

class ConstantMedium : public Hittable
{
public:
	ConstantMedium(std::shared_ptr<Hittable> boundary, float density, std::shared_ptr<Texture> tex)
		: boundary(boundary), negInvDensity(-1.0f / density), phaseFunction(std::make_shared<Isotropic>(tex)) {}
	ConstantMedium(std::shared_ptr<Hittable> boundary, float density, const Color& albedo)
		: boundary(boundary), negInvDensity(-1.0f / density), phaseFunction(std::make_shared<Isotropic>(albedo)) {}

	bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
	{
		// Find where the ray enters and exits the boundary
		HitRecord record1, record2;

		if (!boundary->hit(ray, Interval::Universe, record1))
			return false;

		if (!boundary->hit(ray, Interval(record1.t + 0.0001f, infinity), record2))
			return false;

		// Clamp the entry/exit points to the valid ray interval
		if (record1.t < rayT.min)
			record1.t = rayT.min;
		if (record2.t > rayT.max)
			record2.t = rayT.max;

		if (record1.t >= record2.t)
			return false;

		if (record1.t < 0)
			return false;

		// Convert from parameter-space (t) to actual world-space distance
		auto rayLength = mag(ray.dir);
		auto distanceInsideBoundary = (record2.t - record1.t) * rayLength;

		// Sample a random scattering distance using an exponential distribution
		// Dense media tend to scatter sooner; spare media tend to let rays travel further
		auto hitDistance = negInvDensity * std::log(randomFloat());

		// Check if scattering occurs after leaving the volume
		if (hitDistance > distanceInsideBoundary)
			return false;

		// Convert the scattering distance back into a ray parameter
		record.t = record1.t + hitDistance / rayLength;
		record.p = ray.at(record.t);

		// No actual surface inside a volume, so set arbitrary values
		record.normal = Vec3(1.0f, 0.0f, 0.0f);
		record.frontFace = true;

		record.mat = phaseFunction;

		return true;
	}

	AABB boundingBox() const override
	{
		return boundary->boundingBox();
	}

private:
	std::shared_ptr<Hittable> boundary;
	float negInvDensity;
	std::shared_ptr<Material> phaseFunction;
};