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

class Metal : public Material {
private:
	Color albedo;
	float fuzz;

public:
	Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(
		const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered
	) const override {
		Vec3 reflected = reflect(rayIn.dir, record.normal);
		reflected = normalize(reflected) + (fuzz * randomUnitVector());
		scattered = Ray(record.p, reflected);
		attenuation = albedo;
		return (dot(scattered.dir, record.normal) > 0);
	}
};