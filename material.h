#pragma once

#include "hittable.h"

class Material {
public:
	virtual bool scatter(
		const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered
	) const {
		return false;
	}
};