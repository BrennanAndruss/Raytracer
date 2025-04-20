#pragma once

class Texture
{
public:
	virtual ~Texture() = default;
	virtual Color value(float u, float v, const Point3& p) const = 0;
};

class SolidColor : public Texture
{
private:
	Color albedo;

public:
	SolidColor(const Color& albedo) : albedo(albedo) {}
	SolidColor(float r, float g, float b) : SolidColor(Color(r, g, b)) {}

	Color value(float u, float v, const Point3& p) const override
	{
		return albedo;
	}
};

class CheckerTexture : public Texture
{
private:
	float invScale;
	std::shared_ptr<Texture> even;
	std::shared_ptr<Texture> odd;

public:
	CheckerTexture(float scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
		: invScale(1.0f / scale), even(even), odd(odd) {}
	CheckerTexture(float scale, const Color& c1, const Color& c2)
		: CheckerTexture(scale, std::make_shared<SolidColor>(c1), std::make_shared<SolidColor>(c2)) {}

	Color value(float u, float v, const Point3& p) const override
	{
		int x = static_cast<int>(std::floor(invScale * p.x));
		int y = static_cast<int>(std::floor(invScale * p.y));
		int z = static_cast<int>(std::floor(invScale * p.z));

		bool isEven = (x + y + z) % 2 == 0;

		// Set the value of the even and odd textures based on their texture implementations
		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
	}
};