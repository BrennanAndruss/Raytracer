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

class Lambertian : public Material {
private:
	Color albedo;

public:
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool scatter(
		const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered
	) const override {
		auto scatterDir = record.normal + randomUnitVector();

		if (scatterDir.nearZero()) {
			scatterDir = record.normal;
		}
		
		scattered = Ray(record.p, scatterDir);
		attenuation = albedo;
		return true;
	}
};