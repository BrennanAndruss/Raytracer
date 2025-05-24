#pragma once

#include "hittable.h"
#include "texture.h"

class Material 
{
public:
	virtual ~Material() = default;

	virtual Color emitted(float u, float v, const Point3& p) const
	{
		return Color(0.0f, 0.0f, 0.0f);
	}

	virtual bool scatter(
		const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered
	) const 
	{
		return false;
	}
};

class Lambertian : public Material 
{
private:
	std::shared_ptr<Texture> texture;

public:
	Lambertian(const Color& albedo) : texture(std::make_shared<SolidColor>(albedo)) {}
	Lambertian(std::shared_ptr<Texture> texture) : texture(texture) {}

	bool scatter(
		const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered
	) const override 
	{
		auto scatterDir = record.normal + randomUnitVector();

		if (scatterDir.nearZero())
			scatterDir = record.normal;
		
		scattered = Ray(record.p, scatterDir, rayIn.time);
		attenuation = texture->value(record.u, record.v, record.p);
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
	) const override 
	{
		Vec3 reflected = reflect(rayIn.dir, record.normal);
		reflected = normalize(reflected) + (fuzz * randomUnitVector());
		scattered = Ray(record.p, reflected, rayIn.time);
		attenuation = albedo;
		return (dot(scattered.dir, record.normal) > 0);
	}
};

class Dielectric : public Material 
{
private:
	float refractionIndex;

public:
	Dielectric(float refractionIndex) : refractionIndex(refractionIndex) {}

	bool scatter(
		const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered
	) const override 
	{
		float refractionRatio = record.frontFace ? (1.0f / refractionIndex) : refractionIndex;

		Vec3 unitDir = normalize(rayIn.dir);
		float cosTheta = std::fmin(dot(-unitDir, record.normal), 1.0f);
		float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0f;
		Vec3 direction;

		if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomFloat()) 
		{
			direction = reflect(unitDir, record.normal);
		}
		else 
		{
			direction = refract(unitDir, record.normal, refractionRatio);
		}

		scattered = Ray(record.p, direction, rayIn.time);
		attenuation = Color(1.0f, 1.0f, 1.0f);
		return true;
	}

private:
	static float reflectance(float cosine, float refractionRatio) 
	{
		float r0 = (1 - refractionRatio) / (1 + refractionRatio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};

class DiffuseLight : public Material 
{
private:
	std::shared_ptr<Texture> tex;

public:
	DiffuseLight(std::shared_ptr<Texture> tex) : tex(tex) {}
	DiffuseLight(const Color& emit) : tex(std::make_shared<SolidColor>(emit)) {}

	color emitted(float u, float v, const Point3& p) const override
	{
		return tex->value(u, v, p);
	}
};