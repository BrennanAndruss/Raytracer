#include "raytracer.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

void bouncingSpheres() {

	HittableList world;

	auto checker = std::make_shared<CheckerTexture>(
		0.32f, Color(0.2f, 0.3f, 0.1f), Color(0.9f, 0.9f, 0.9f));
	world.add(std::make_shared<Sphere>(
		Point3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(checker)));

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

	world = HittableList(std::make_shared<BVHNode>(world));

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

	camera.render(world);
}

void checkeredSpheres()
{
	HittableList world;

	auto checker = std::make_shared<CheckerTexture>(
		0.32f, Color(0.2f, 0.3f, 0.1f), Color(0.9f, 0.9f, 0.9f));

	world.add(std::make_shared<Sphere>(
		Point3(0.0f, -10.0f, 0.0f), 10.0f, std::make_shared<Lambertian>(checker)));
	world.add(std::make_shared<Sphere>(
		Point3(0.0f, 10.0f, 0.0f), 10.0f, std::make_shared<Lambertian>(checker)));

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 50;

	camera.vFov = 20;
	camera.lookFrom = Point3(13.0f, 2.0f, 3.0f);
	camera.lookAt = Point3(0.0f, 0.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0;

	camera.render(world);
}

void earth()
{
	auto earthTexture = std::make_shared<ImageTexture>("earthmap.jpg");
	auto earthMaterial = std::make_shared<Lambertian>(earthTexture);
	auto globe = std::make_shared<Sphere>(Point3(0.0f, 0.0f, 0.0f), 2.0f, earthMaterial);

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 50;

	camera.vFov = 20;
	camera.lookFrom = Point3(0.0f, 0.0f, 12.0f);
	camera.lookAt = Point3(0.0f, 0.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0;

	camera.render(HittableList(globe));
}

int main(int argc, char* argv[])
{
	// Select the scene to render using command line arguments
	if (argc < 2)
	{
		// Prompt the user to rerun the program with a scene number if none is provided
		std::cerr << "Provide an integer as the scene number to render.\n";
		return 1;
	}

	int scene;
	try
	{
		scene = std::stoi(argv[1]);
	}
	catch (std::invalid_argument&)
	{
		std::cerr << "Invalid input. Please provide a valid integer.\n";
		return 1;
	}

	// Capture the render time of the selected scene
	auto start = std::chrono::high_resolution_clock::now();

	switch (scene)
	{
	case 1: bouncingSpheres(); break;
	case 2: checkeredSpheres(); break;
	case 3: earth(); break;
	default: bouncingSpheres(); break;
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;
	std::clog << "Render time: " << duration.count() << " s\n";
}