#pragma once

#include "hittable_list.h"
#include "orthonormal_basis.h"

#include <cmath>

class PDF
{
public:
	virtual ~PDF() {}

	virtual float value(const Vec3& direction) const = 0;
	virtual Vec3 generate() const = 0;
};

class SpherePDF : public PDF
{
public:
	SpherePDF() {}

	float value(const Vec3& direction) const override
	{
		return 1.0f / (4.0f * pi);
	}

	Vec3 generate() const override
	{
		return randomUnitVector();
	}
};

class CosinePDF : public PDF
{
public:
	CosinePDF(const Vec3& w) : uvw(w) {}

	float value(const Vec3& direction) const override
	{
		float cosineTheta = dot(normalize(direction), uvw.w());
		return std::fmax(0, cosineTheta / pi);
	}

	Vec3 generate() const override
	{
		return uvw.transform(randomCosineDirection());
	}

private:
	OrthonormalBasis uvw;
};

class HittablePDF : public PDF
{
public:
	HittablePDF(const Hittable& objects, const Point3& origin)
		: objects(objects), origin(origin) {}

	float value(const Vec3& direction) const override
	{
		return objects.pdfValue(origin, direction);
	}

	Vec3 generate() const override
	{
		return objects.random(origin);
	}

private:
	const Hittable& objects;
	Point3 origin;
};

class MixturePDF : public PDF
{
public:
	MixturePDF(
		std::vector<std::shared_ptr<PDF>> pdfs, std::vector<float> weights)
		: p(std::move(pdfs)), weights(std::move(weights))
	{
		// TODO: input validation

		float totalWeight = 0.0f;
		for (int i = 0; i < p.size(); i++)
		{
			totalWeight += this->weights[i];
		}

		for (float& weight : this->weights)
		{
			weight /= totalWeight;
		}
	}

	float value(const Vec3& direction) const override
	{
		float result = 0.0f;
		for (int i = 0; i < p.size(); i++)
		{
			result += weights[i] * p[i]->value(direction);
		}
		
		return result;
	}

	Vec3 generate() const override
	{
		float r = randomFloat();
		float cumulative = 0.0f;

		for (size_t i = 0; i < p.size(); i++)
		{
			cumulative += weights[i];

			if (r < cumulative)
			{
				return p[i]->generate();
			}
		}

		return p.back()->generate();
	}

private:
	std::vector<std::shared_ptr<PDF>> p;
	std::vector<float> weights;
};