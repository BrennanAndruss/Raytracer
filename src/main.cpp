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
	auto materialLeft = std::make_shared<Dielectric>(1.50f);
	auto materialBubble = std::make_shared<Dielectric>(1.00f / 1.50f);
	auto materialRight = std::make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 1.0f);

	world.add(std::make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	world.add(std::make_shared<Sphere>(Point3(0.0f, 0.0f, -1.2f), 0.5f, materialCenter));
	world.add(std::make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
	world.add(std::make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), 0.4f, materialBubble));
	world.add(std::make_shared<Sphere>(Point3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 50;

	camera.vFov = 20.0f;
	camera.lookFrom = Point3(-2.0f, 2.0f, 1.0f);
	camera.lookAt = Point3(0.0f, 0.0f, -1.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 10.0f;
	camera.focusDist = 3.4f;

	camera.render(world);
	
}