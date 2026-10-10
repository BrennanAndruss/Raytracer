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
	MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1)
	{
		p[0] = p0;
		p[1] = p1;
	}

	float value(const Vec3& direction) const override
	{
		return 0.5f * p[0]->value(direction) + 0.5f * p[1]->value(direction);
	}

	Vec3 generate() const override
	{
		if (randomFloat() < 0.5f)
			return p[0]->generate();
		else
			return p[1]->generate();
	}

private:
	std::shared_ptr<PDF> p[2];
};