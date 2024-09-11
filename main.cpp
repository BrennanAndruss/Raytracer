#include "raytracer.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {

	HittableList world;

	world.add(std::make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f));
	world.add(std::make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f));

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 400;

	camera.render(world);
	
}