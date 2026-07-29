#pragma once

#include "raytracer.h"

#include <array>

class Perlin
{
public:
	Perlin()
	{
		for (int i = 0; i < pointCount; i++)
		{
			randVec[i] = normalize(Vec3::random(-1.0f, 1.0f));
		}

		perlinGeneratePerm(permX);
		perlinGeneratePerm(permY);
		perlinGeneratePerm(permZ);
	}

	float noise(const Point3& p) const
	{
		// Compute the point's local coordinates within its current lattice cell
		// Points are in [0, 1)
		auto u = p.x - std::floor(p.x);
		auto v = p.y - std::floor(p.y);
		auto w = p.z - std::floor(p.z);

		// Find the integer lattice cell containing p
		auto i = int(std::floor(p.x));
		auto j = int(std::floor(p.y));
		auto k = int(std::floor(p.z));

		// Generate random gradient vectors at the eight corners of the 
		// surrounding lattice cell
		Vec3 corners[2][2][2];
		for (int di = 0; di < 2; di++)
		{
			for (int dj = 0; dj < 2; dj++)
			{
				for (int dk = 0; dk < 2; dk++)
				{
					// Hash the lattice coordinate into a pseudorandom index
					corners[di][dj][dk] = randVec[
						permX[(i + di) & 255] ^
							permY[(j + dj) & 255] ^
							permZ[(k + dk) & 255]
					];
				}
			}
		}

		// Compute the gradient noise
		return perlinInterpolate(corners, u, v, w);
	}

	float turbulence(const Point3& p, int depth) const
	{
		auto accum = 0.0f;
		auto tempP = p;
		auto weight = 1.0f;

		// Sum octaves of Perlin noise
		// For each octave, double the frequency and half the contribution
		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(tempP);
			weight *= 0.5;
			tempP *= 2;
		}

		return std::fabs(accum);
	}

private:
	static const int pointCount = 256;
	std::array<Vec3, pointCount> randVec;
	std::array<int, pointCount> permX;
	std::array<int, pointCount> permY;
	std::array<int, pointCount> permZ;

	static void perlinGeneratePerm(std::array<int, pointCount>& p)
	{
		for (int i = 0; i < pointCount; i++)
		{
			p[i] = i;
		}

		permute(p);
	}

	static void permute(std::array<int, pointCount>& p)
	{
		for (int i = p.size() - 1; i > 0; i--)
		{
			int target = randomInt(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static float perlinInterpolate(const Vec3 corners[2][2][2], 
		float u, float v, float w)
	{
		// Perform smoothstep interpolation
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);

		auto accum = 0.0f;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					// Compute vector from this lattice corner to the sample point
					Vec3 weightV(u - i, v - j, w - k);

					// Interpolate weight for this corner with trilinear interpolation
					float interpWeight =
						(i * uu + (1 - i) * (1 - uu)) *
						(j * vv + (1 - j) * (1 - vv)) *
						(k * ww + (1 - k) * (1 - ww));
					
					// Compute this corner's contribution based on the how well the 
					// gradient vector points toward the sample
					float influence = dot(corners[i][j][k], weightV);

					accum += interpWeight * influence;
				}
			}
		}

		return accum;
	}
};