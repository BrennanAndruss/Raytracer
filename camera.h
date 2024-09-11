#pragma once

#include "hittable.h"

class Camera {
public:
	float aspectRatio = 1.0f;
	int imageWidth = 100;

	void render(const Hittable& world) {
		initialize();

		std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++) {
			std::clog << "\rScanlines remaining: " << (imageHeight - j) << " " << std::flush;
			for (int i = 0; i < imageWidth; i++) {
				auto pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
				auto rayDir = pixelCenter - center;
				Ray ray(center, rayDir);

				color pixelColor = rayColor(ray, world);
				writeColor(std::cout, pixelColor);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	int imageHeight;
	Point3 center;
	Point3 pixel00Loc;
	Vec3 pixelDeltaU;
	Vec3 pixelDeltaV;

	void initialize() {
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		center = Point3(0.0f, 0.0f, 0.0f);

		// Determine viewport dimensions
		float focalLength = 1.0f;
		float viewportHeight = 2.0f;
		float viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);

		// Calculate the horizontal and vertical vectors along the viewport edges
		auto viewportU = Vec3(viewportWidth, 0.0f, 0.0f);
		auto viewportV = Vec3(0.0f, -viewportHeight, 0.0f);

		// Calculate the horizontal and vertical delta vectors from pixel to pixel
		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		// Calculate the location of the upper left pixel
		auto viewportUpperLeft =
			center - Vec3(0.0f, 0.0f, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
		pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
	}

	Color rayColor(const Ray& ray, const Hittable& world) const {
		HitRecord record;

		if (world.hit(ray, Interval(0, infinity), record)) {
			return 0.5 * (record.normal + Color(1.0f, 1.0f, 1.0f));
		}

		Vec3 unitDir = normalize(ray.dir);
		float a = 0.5 * (unitDir.y + 1.0f);
		return (1.0 - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
	}
};