#include "raytracer.h"

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"

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

#define useBVH 1
#if useBVH
	world = HittableList(std::make_shared<BVHNode>(world));
#endif

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 1200;
	camera.samplesPerPixel = 500;
	camera.maxDepth = 50;
	camera.background = Color(0.7f, 0.8f, 1.0f);

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
	camera.background = Color(0.7f, 0.8f, 1.0f);

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
	camera.background = Color(0.7f, 0.8f, 1.0f);

	camera.vFov = 20;
	camera.lookFrom = Point3(0.0f, 0.0f, 12.0f);
	camera.lookAt = Point3(0.0f, 0.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0;

	camera.render(HittableList(globe));
}

void perlinSpheres()
{
	HittableList world;

	auto perTex = std::make_shared<NoiseTexture>(4.0f);
	world.add(std::make_shared<Sphere>(Point3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(perTex)));
	world.add(std::make_shared<Sphere>(Point3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<Lambertian>(perTex)));

	Camera camera;

	camera.aspectRatio = 16.0f / 9.0f;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 50;
	camera.background = Color(0.7f, 0.8f, 1.0f);

	camera.vFov = 20;
	camera.lookFrom = Point3(13.0f, 2.0f, 3.0f);
	camera.lookAt = Point3(0.0f, 0.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0;
	
	camera.render(world);
}

void quads()
{
	HittableList world;

	auto leftRed = std::make_shared<Lambertian>(Color(1.0f, 0.2f, 0.2f));
	auto backGreen = std::make_shared<Lambertian>(Color(0.2f, 1.0f, 0.2f));
	auto rightBlue = std::make_shared<Lambertian>(Color(0.2f, 0.2f, 1.0f));
	auto upperOrange = std::make_shared<Lambertian>(Color(1.0f, 0.5f, 0.0f));
	auto lowerTeal = std::make_shared<Lambertian>(Color(0.2f, 0.8f, 0.8f));

	world.add(std::make_shared<Quad>(Point3(-3.0f, -2.0f, 5.0f), Vec3(0.0f, 0.0f, -4.0f),
		Vec3(0.0f, 4.0f, 0.0f), leftRed));
	world.add(std::make_shared<Quad>(Point3(-2.0f, -2.0f, 0.0f), Vec3(4.0f, 0.0f, 0.0f),
		Vec3(0.0f, 4.0f, 0.0f), backGreen));
	world.add(std::make_shared<Quad>(Point3(3.0f, -2.0f, 1.0f), Vec3(0.0f, 0.0f, 4.0f),
		Vec3(0.0f, 4.0f, 0.0f), rightBlue));
	world.add(std::make_shared<Quad>(Point3(-2.0f, 3.0f, 1.0f), Vec3(4.0f, 0.0f, 0.0f),
		Vec3(0.0f, 0.0f, 4.0f), upperOrange));
	world.add(std::make_shared<Quad>(Point3(-2.0f, -3.0f, 5.0f), Vec3(4.0f, 0.0f, 0.0f),
		Vec3(0.0f, 0.0f, -4.0f), lowerTeal));

	Camera camera;

	camera.aspectRatio = 1.0;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 50;
	camera.background = Color(0.7f, 0.8f, 1.0f);

	camera.vFov = 80;
	camera.lookFrom = Point3(0.0f, 0.0f, 9.0f);
	camera.lookAt = Point3(0.0f, 0.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);
	camera.defocusAngle = 0;

	camera.render(world);
}

void cornellBox()
{
	HittableList world;

	auto red = std::make_shared<Lambertian>(Color(0.65f, 0.05f, 0.05f));
	auto white = std::make_shared<Lambertian>(Color(0.73f, 0.73f, 0.73f));
	auto green = std::make_shared<Lambertian>(Color(0.12f, 0.45f, 0.15f));
	auto light = std::make_shared<DiffuseLight>(Color(15.0f, 15.0f, 15.0f));

	world.add(std::make_shared<Quad>(Point3(555.0f, 0.0f, 0.0f), 
		Vec3(0.0f, 555.0f, 0.0f), Vec3(0.0f, 0.0f, 555.0f), green));
	world.add(std::make_shared<Quad>(Point3(0.0f, 0.0f, 0.0f), 
		Vec3(0.0f, 555.0f, 0.0f), Vec3(0.0f, 0.0f, 555.0f), red));
	world.add(std::make_shared<Quad>(Point3(343.0f, 554.0f, 332.0f), 
		Vec3(-130.0f, 0.0f, 0.0f), Vec3(0, 0, -105), light));
	world.add(std::make_shared<Quad>(Point3(0.0f, 0.0f, 0.0f), 
		Vec3(555.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 555.0f), white));
	world.add(std::make_shared<Quad>(Point3(555.0f, 555.0f, 555.0f), 
		Vec3(-555.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -555.0f), white));
	world.add(std::make_shared<Quad>(Point3(0.0f, 0.0f, 555.0f), 
		Vec3(555.0f, 0.0f, 0.0f), Vec3(0.0f, 555.0f, 0.0f), white));

	std::shared_ptr<Hittable> box1 = box(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 330.0f, 165.0f), white);
	box1 = std::make_shared<RotateY>(box1, 15.0f);
	box1 = std::make_shared<Translate>(box1, Vec3(265.0f, 0.0f, 295.0f));
	world.add(box1);

	std::shared_ptr<Hittable> box2 = box(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 165.0f, 165.0f), white);
	box2 = std::make_shared<RotateY>(box2, -18.0f);
	box2 = std::make_shared<Translate>(box2, Vec3(130.0f, 0.0f, 65.0f));
	world.add(box2);

	Camera camera;

	camera.aspectRatio = 1.0f;
	camera.imageWidth = 1200;
	camera.samplesPerPixel = 500;
	camera.maxDepth = 50;
	camera.background = Color(0.0f, 0.0f, 0.0f);

	camera.vFov = 40.0f;
	camera.lookFrom = Point3(278.0f, 278.0f, -800.0f);
	camera.lookAt = Point3(278.0f, 278.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	camera.render(world);
}

void cornellSmoke()
{
	HittableList world;

	auto red = std::make_shared<Lambertian>(Color(0.65f, 0.05f, 0.05f));
	auto white = std::make_shared<Lambertian>(Color(0.73f, 0.73f, 0.73f));
	auto green = std::make_shared<Lambertian>(Color(0.12f, 0.45f, 0.15f));
	auto light = std::make_shared<DiffuseLight>(Color(15.0f, 15.0f, 15.0f));

	world.add(std::make_shared<Quad>(Point3(555.0f, 0.0f, 0.0f),
		Vec3(0.0f, 555.0f, 0.0f), Vec3(0.0f, 0.0f, 555.0f), green));
	world.add(std::make_shared<Quad>(Point3(0.0f, 0.0f, 0.0f),
		Vec3(0.0f, 555.0f, 0.0f), Vec3(0.0f, 0.0f, 555.0f), red));
	world.add(std::make_shared<Quad>(Point3(343.0f, 554.0f, 332.0f),
		Vec3(-130.0f, 0.0f, 0.0f), Vec3(0, 0, -105), light));
	world.add(std::make_shared<Quad>(Point3(0.0f, 0.0f, 0.0f),
		Vec3(555.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 555.0f), white));
	world.add(std::make_shared<Quad>(Point3(555.0f, 555.0f, 555.0f),
		Vec3(-555.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -555.0f), white));
	world.add(std::make_shared<Quad>(Point3(0.0f, 0.0f, 555.0f),
		Vec3(555.0f, 0.0f, 0.0f), Vec3(0.0f, 555.0f, 0.0f), white));

	std::shared_ptr<Hittable> box1 = box(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 330.0f, 165.0f), white);
	box1 = std::make_shared<RotateY>(box1, 15.0f);
	box1 = std::make_shared<Translate>(box1, Vec3(265.0f, 0.0f, 295.0f));
	world.add(std::make_shared<ConstantMedium>(box1, 0.01f, Color(0.0f, 0.0f, 0.0f)));

	std::shared_ptr<Hittable> box2 = box(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 165.0f, 165.0f), white);
	box2 = std::make_shared<RotateY>(box2, -18.0f);
	box2 = std::make_shared<Translate>(box2, Vec3(130.0f, 0.0f, 65.0f));
	world.add(std::make_shared<ConstantMedium>(box2, 0.01f, Color(1.0f, 1.0f, 1.0f)));

	Camera camera;

	camera.aspectRatio = 1.0f;
	camera.imageWidth = 600;
	camera.samplesPerPixel = 200;
	camera.maxDepth = 50;
	camera.background = Color(0.0f, 0.0f, 0.0f);

	camera.vFov = 40.0f;
	camera.lookFrom = Point3(278.0f, 278.0f, -800.0f);
	camera.lookAt = Point3(278.0f, 278.0f, 0.0f);
	camera.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	camera.defocusAngle = 0.0f;

	camera.render(world);
}

void finalScene(int imageWidth, int samplesPerPixel, int maxDepth)
{
	HittableList boxes1;
	auto ground = std::make_shared<Lambertian>(Color(0.48f, 0.83f, 0.53f));

	int boxesPerSide = 20;
	for (int i = 0; i < boxesPerSide; i++)
	{
		for (int j = 0; j < boxesPerSide; j++)
		{
			auto w = 100.0f;
			auto x0 = -1000.0f + i * w;
			auto z0 = -1000.0f + j * w;
			auto y0 = 0.0f;
			auto x1 = x0 + w;
			auto y1 = randomFloat(1.0f, 101.0f);
			auto z1 = z0 + w;

			boxes1.add(box(Point3(z0, y0, z0), Point3(x1, y1, z1), ground));
		}
	}

	HittableList world;

	world.add(std::make_shared<BVHNode>(boxes1));

	auto light = std::make_shared<DiffuseLight>(Color(7.0f, 7.0f, 7.0f));
	world.add(std::make_shared<Quad>(Point3(123.0f, 554.0f, 147.0f), Vec3(300.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 265.0f), light));

	auto center1 = Point3(400.0f, 400.0f, 200.0f);
	auto center2 = center1 + Vec3(30.0f, 0.0f, 0.0f);
	auto sphere_material = std::make_shared<Lambertian>(Color(0.7f, 0.3f, 0.1f));
	world.add(std::make_shared<Sphere>(center1, center2, 50.0f, sphere_material));

	world.add(std::make_shared<Sphere>(Point3(260.0f, 150.0f, 45.0f), 50.0f, std::make_shared<Dielectric>(1.5)));
	world.add(std::make_shared<Sphere>(
		Point3(0.0f, 150.0f, 145.0f), 50.0f, std::make_shared<Metal>(Color(0.8f, 0.8f, 0.9f), 1.0f)
	));

	auto boundary = std::make_shared<Sphere>(Point3(360.0f, 150.0f, 145.0f), 70.0f, std::make_shared<Dielectric>(1.5));
	world.add(boundary);
	world.add(std::make_shared<ConstantMedium>(boundary, 0.2f, Color(0.2f, 0.4f, 0.9f)));
	boundary = std::make_shared<Sphere>(Point3(0.0f, 0.0f, 0.0f), 5000.0f, std::make_shared<Dielectric>(1.5));
	world.add(std::make_shared<ConstantMedium>(boundary, .0001f, Color(1.0f, 1.0f, 1.0f)));

	auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
	world.add(std::make_shared<Sphere>(Point3(400.0f, 200.0f, 400.0f), 100.0f, emat));
	auto pertext = std::make_shared<NoiseTexture>(0.2f);
	world.add(std::make_shared<Sphere>(Point3(220.0f, 280.0f, 300.0f), 80.0f, std::make_shared<Lambertian>(pertext)));

	HittableList boxes2;
	auto white = std::make_shared<Lambertian>(Color(0.73f, 0.73f, 0.73f));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxes2.add(std::make_shared<Sphere>(Point3::random(0.0f, 165.0f), 10.0f, white));
	}

	world.add(std::make_shared<Translate>(
		std::make_shared<RotateY>(
			std::make_shared<BVHNode>(boxes2), 15.0f),
		Vec3(-100.0f, 270.0f, 395.0f)
	)
	);

	Camera cam;

	cam.aspectRatio = 1.0;
	cam.imageWidth = imageWidth;
	cam.samplesPerPixel = samplesPerPixel;
	cam.maxDepth = maxDepth;
	cam.background = Color(0.0f, 0.0f, 0.0f);

	cam.vFov = 40;
	cam.lookFrom = Point3(478.0f, 278.0f, -600.0f);
	cam.lookAt = Point3(278.0f, 278.0f, 0.0f);
	cam.viewUp = Vec3(0.0f, 1.0f, 0.0f);

	cam.defocusAngle = 0.0f;

	cam.render(world);
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
	case 4: perlinSpheres(); break;
	case 5: quads(); break;
	case 6: cornellBox(); break;
	case 7: cornellSmoke(); break;
	case 8: finalScene(800, 10000, 40); break;
	default: finalScene(400, 250, 4); break;
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;
	std::clog << "Render time: " << duration.count() << " s\n";
}