#pragma once

#include "hittable.h"
#include "material.h"

class Camera {
public:
	float aspectRatio = 1.0f;
	int imageWidth = 100;
	int samplesPerPixel = 10;
	int maxDepth = 10;

	float vFov = 90.0f;
	Point3 lookFrom = Point3(0.0f, 0.0f, 0.0f);
	Point3 lookAt = Point3(0.0f, 0.0f, -1.0f);
	Vec3 viewUp = Vec3(0.0f, 1.0f, 0.0f);

	void render(const Hittable& world) {
		initialize();

		std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++) {
			std::clog << "\rScanlines remaining: " << (imageHeight - j) << " " << std::flush;
			for (int i = 0; i < imageWidth; i++) {
				Color pixelColor(0.0f, 0.0f, 0.0f);

				for (int sample = 0; sample < samplesPerPixel; sample++) {
					Ray ray = getRay(i, j);
					pixelColor += rayColor(ray, maxDepth, world);
				}

				writeColor(std::cout, pixelSamplesScale * pixelColor);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	int imageHeight;
	float pixelSamplesScale;	// Color scale factor for a sum of pixel samples
	Point3 center;
	Point3 pixel00Loc;
	Vec3 pixelDeltaU;
	Vec3 pixelDeltaV;
	Vec3 u, v, w;				// Camera frame basis vectors

	void initialize() {
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		pixelSamplesScale = 1.0f / samplesPerPixel;

		center = lookFrom;

		// Determine viewport dimensions
		float focalLength = mag(lookFrom - lookAt);
		float theta = degreesToRadians(vFov);
		float h = std::tan(theta / 2.0f);
		float viewportHeight = 2.0f * h * focalLength;
		float viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);

		// Calculate the unit basis vectors for the camera coordinate frame
		w = normalize(lookFrom - lookAt);
		u = normalize(cross(viewUp, w));
		v = cross(w, u);

		// Calculate the horizontal and vertical vectors along the viewport edges
		auto viewportU = viewportWidth * u;
		auto viewportV = viewportHeight * -v;

		// Calculate the horizontal and vertical delta vectors from pixel to pixel
		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		// Calculate the location of the upper left pixel
		auto viewportUpperLeft = 
			center - (focalLength * w) - viewportU / 2.0f - viewportV / 2.0f;
		pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
	}

	Ray getRay(int i, int j) const {
		// Construct a camera ray originating from the origin and driected at randomly sampled 
		// point around the pixel location i, j

		Vec3 offset = sampleSquare();
		Point3 pixelSample = pixel00Loc + (i + offset.x) * pixelDeltaU + (j + offset.y) * pixelDeltaV;

		Point3 rayOrigin = center;
		Vec3 rayDir = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDir);
	}

	Vec3 sampleSquare() const {
		// Returns the vector to a random point in the [-.5, -.5]-[+.5, +.5] square
		return Vec3(randomFloat() - 0.5f, randomFloat() - 0.5f, 0.0f);
	}

	Color rayColor(const Ray& ray, int depth, const Hittable& world) const {
		// If the ray bounce limit is exceeded, nor more light is gathered
		if (depth <= 0) {
			return Color(0.0f, 0.0f, 0.0f);
		}

		HitRecord record;

		if (world.hit(ray, Interval(0.001f, infinity), record)) {
			Ray scattered;
			Color attenuation;
			if (record.mat->scatter(ray, record, attenuation, scattered)) {
				return attenuation * rayColor(scattered, depth - 1, world);
			}
			return Color(0.0f, 0.0f, 0.0f);
		}

		Vec3 unitDir = normalize(ray.dir);
		float a = 0.5 * (unitDir.y + 1.0f);
		return (1.0 - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
	}
};