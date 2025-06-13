#pragma once

#include "hittable.h"
#include "material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class Camera 
{
public:
	float aspectRatio = 1.0f;
	int imageWidth = 100;
	int samplesPerPixel = 10;
	int maxDepth = 10;
	Color background;

	float vFov = 90.0f;
	Point3 lookFrom = Point3(0.0f, 0.0f, 0.0f);
	Point3 lookAt = Point3(0.0f, 0.0f, -1.0f);
	Vec3 viewUp = Vec3(0.0f, 1.0f, 0.0f);

	float defocusAngle = 0.0f;
	float focusDist = 1.0f;

	void render(const Hittable& world) 
	{
		initialize();

		// std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++) 
		{
			std::clog << "\rScanlines remaining: " << (imageHeight - j) << " " << std::flush;
			for (int i = 0; i < imageWidth; i++) 
			{
				Color pixelColor(0.0f, 0.0f, 0.0f);

				for (int sample = 0; sample < samplesPerPixel; sample++) 
				{
					Ray ray = getRay(i, j);
					pixelColor += rayColor(ray, maxDepth, world);
					
				}

				setPixel(i, j, pixelSamplesScale * pixelColor);
			}
		}

		std::clog << "\rDone.                 \n";
		writePNG("output.png");
	}

private:
	int imageHeight;
	float pixelSamplesScale;			// Color scale factor for a sum of pixel samples
	std::vector<Color> pixels;

	Point3 center;
	Point3 pixel00Loc;
	Vec3 pixelDeltaU, pixelDeltaV;
	Vec3 u, v, w;						// Camera frame basis vectors
	Vec3 defocusDiskU, defocusDiskV;

	void initialize() 
	{
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;
		pixels.resize(imageWidth * imageHeight, Color(0.0f, 0.0f, 0.0f));

		pixelSamplesScale = 1.0f / samplesPerPixel;

		center = lookFrom;

		// Determine viewport dimensions
		float theta = degreesToRadians(vFov);
		float h = std::tan(theta / 2.0f);
		float viewportHeight = 2.0f * h * focusDist;
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
			center - (focusDist * w) - viewportU / 2.0f - viewportV / 2.0f;
		pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

		// Calculate the camera defocus disk basis vectors
		float defocusRadius = focusDist * std::tan(degreesToRadians(defocusAngle) / 2.0f);
		defocusDiskU = defocusRadius * u;
		defocusDiskV = defocusRadius * v;
	}

	Ray getRay(int i, int j) const 
	{
		// Construct a camera ray originating from the defocus disk and driected at randomly
		// sampled point around the pixel location i, j

		Vec3 offset = sampleSquare();
		Point3 pixelSample = 
			pixel00Loc + (i + offset.x) * pixelDeltaU + (j + offset.y) * pixelDeltaV;

		Point3 rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
		Vec3 rayDir = pixelSample - rayOrigin;
		float rayTime = randomFloat();

		return Ray(rayOrigin, rayDir, rayTime);
	}

	Vec3 sampleSquare() const 
	{
		// Returns the vector to a random point in the [-.5, -.5]-[+.5, +.5] square
		return Vec3(randomFloat() - 0.5f, randomFloat() - 0.5f, 0.0f);
	}

	Point3 defocusDiskSample() const 
	{
		// Returns a random point in the camera defocus disk
		Point3 p = randomInUnitDisk();
		return center + p.x * defocusDiskU + p.y * defocusDiskV;
	}

	Color rayColor(const Ray& ray, int depth, const Hittable& world) const 
	{
		// If the ray bounce limit is exceeded, no more light is gathered
		if (depth <= 0) {
			return Color(0.0f, 0.0f, 0.0f);
		}

		HitRecord record;

		// If the ray hits noting, return the background color
		if (!world.hit(ray, Interval(0.001f, infinity), record))
			return background;

		Ray scattered;
		Color attenuation;
		Color emissionColor = record.mat->emitted(record.u, record.v, record.p);
		
		if (!record.mat->scatter(ray, record, attenuation, scattered))
			return emissionColor;
		
		// If the ray is scattered, recursively gather light from the new ray
		Color scatterColor = attenuation * rayColor(scattered, depth - 1, world);
		return emissionColor + scatterColor;
	}

	void setPixel(int x, int y, Color color)
	{
		// Check for out of bounds
		if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
		{
			return;
		}

		pixels[y * imageWidth + x] += color;
	}

	void writePNG(const std::string& filename) const
	{
		int strideInBytes = imageWidth * 3 * sizeof(unsigned char);
		std::vector<unsigned char> imageData(imageWidth * imageHeight * 3);

		for (int j = 0; j < imageHeight; j++)
		{
			for (int i = 0; i < imageWidth; i++)
			{
				int index = (j * imageWidth + i);

				// Apply a linear to gamma transformation for gamma 2
				auto r = linearToGamma(pixels[index].x);
				auto g = linearToGamma(pixels[index].y);
				auto b = linearToGamma(pixels[index].z);

				index *= 3;
				static const Interval intensity(0.0f, 0.999f);
				imageData[index] = static_cast<unsigned char>(256 * intensity.clamp(r));
				imageData[index + 1] = static_cast<unsigned char>(256 * intensity.clamp(g));
				imageData[index + 2] = static_cast<unsigned char>(256 * intensity.clamp(b));
			}
		}

		int rc = stbi_write_png(filename.c_str(), imageWidth, imageHeight, 
			3, imageData.data(), strideInBytes);
		if (rc)
			std::cout << "Image saved to " << filename << "\n";
		else
			std::cerr << "Failed to save image to " << filename << "\n";
	}
};