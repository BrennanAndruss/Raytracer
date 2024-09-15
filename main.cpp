#include "raytracer.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {

	HittableList world;

	auto materialGround = std::make_shared<Lambertian>(Color(0.8f, 0.8f, 0.0f));
	auto materialCenter = std::make_shared<Lambertian>(Color(0.1f, 0.2f, 0.5f));

	world.add(std::make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	world.add(std::make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f, materialCenter));

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 50;

	camera.render(world);
	
}