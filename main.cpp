#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "color.h"

float hitSphere(const Point3& center, float radius, const Ray& ray) {
	Vec3 oc = center - ray.origin;
	float a = sqrMag(ray.dir);
	float b = -2.0f * dot(ray.dir, oc);
	float c = sqrMag(oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	
	if (discriminant < 0) {
		return -1.0f;
	}
	else {
		return (-b - std::sqrt(discriminant)) / (2.0f * a);
	}
}

Color rayColor(const Ray& ray) {
	auto t = hitSphere(Point3(0.0f, 0.0f, -1.0f), 0.5f, ray);
	if (t > 0.0f) {
		Vec3 N = normalize(ray.at(t) - Vec3(0.0f, 0.0f, -1.0f));
		return 0.5f * Color(N.x + 1.0f, N.y + 1.0f, N.z + 1.0f);
	}

	Vec3 unitDir = normalize(ray.dir);
	float a = 0.5f * (unitDir.y + 1.0f);
	return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
}

int main() {

	// Image

	float aspectRatio = 16.0f / 9.0f;
	int imageWidth = 400;

	// Calculate the image height
	int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	//Camera

	float focalLength = 1.0f;
	float viewportHeight = 2.0f;
	float viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);
	auto cameraCenter = Point3(0.0f, 0.0f, 0.0f);

	// Calculate the horizontal and vertical vectors along the viewport edges
	auto viewportU = Vec3(viewportWidth, 0.0f, 0.0f);
	auto viewportV = Vec3(0.0f, -viewportHeight, 0.0f);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel
	auto pixelDeltaU = viewportU / imageWidth;
	auto pixelDeltaV = viewportV / imageHeight;

	// Calculate the location of the upper left pixel
	auto viewportUpperLeft = 
		cameraCenter - Vec3(0.0f, 0.0f, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
	auto pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

	// Render

	std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	for (int j = 0; j < imageHeight; j++) {
		std::clog << "\rScanlines remaining: " << (imageHeight - j) << " " << std::flush;
		for (int i = 0; i < imageWidth; i++) {
			auto pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
			auto rayDir = pixelCenter - cameraCenter;
			Ray r(cameraCenter, rayDir);

			color pixelColor = rayColor(r);
			writeColor(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone.                 \n";
}