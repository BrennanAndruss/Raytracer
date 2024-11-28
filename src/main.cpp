#include "raytracer.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {

	HittableList world;

	auto materialGround = std::make_shared<Lambertian>(Color(0.5f, 0.5f, 0.5f));
	world.add(std::make_shared<Sphere>(Point3(0.0f, -1000.0f, 0.0f), 1000.0f, materialGround));

	// Add lots of random spheres
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto chooseMat = randomFloat();
			Point3 center(a + 0.9f * randomFloat(), 0.2f, b + 0.9f * randomFloat());

			if (mag(center - Point3(4.0f, 0.2f, 0.0f)) > 0.9f) {
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8f) {
					// Diffuse
					auto albedo = Color::random() * Color::random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					auto center2 = center + Vec3(0.0f, randomFloat(0.0f, 0.5f), 0.0f);
					world.add(std::make_shared<Sphere>(center, center2, 0.2f, sphereMaterial));
				}
				else if (chooseMat < 0.95f) {
					// Metal
					auto albedo = Color::random();
					auto fuzz = randomFloat(0.0f, 0.5f);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else {
					// Glass
					sphereMaterial = std::make_shared<Dielectric>(1.5f);
					world.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5f);
	world.add(std::make_shared<Sphere>(Point3(0.0f, 1.0f, 0.0f), 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(Color(0.4f, 0.2f, 0.1f));
	world.add(std::make_shared<Sphere>(Point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f);
	world.add(std::make_shared<Sphere>(Point3(4.0f, 1.0f, 0.0f), 1.0f, material3));

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 10;

	camera.vFov = 20.0f;
	camera.lookFrom = Point3(13.0f, 2.0f, 3.0f);
	camera.lookAt = Point3(0.0f, 0.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.6f;
	camera.focusDist = 10.0f;

	auto start = std::chrono::high_resolution_clock::now();
	camera.render(world);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;
	std::clog << "Render time: " << duration.count() << " s\n";
	
}