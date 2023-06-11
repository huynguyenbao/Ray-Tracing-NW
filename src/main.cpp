// RT1W.cpp : Defines the entry point for the application.
//

#include "RTNW.h"
#include "Camera.h"
#include "Ray.h"
#include "HittableList.h"
#include "Color.h"
#include "Sphere.h"
#include "Material.h"
#include "MovingSphere.h"
#include "Texture.h"
#include "AARect.h"
#include <iostream>
#include <fstream>
#include "Box.h"
#include "ConstantMedium.h"
#include "BVHNode.h"

using namespace std;

Color rayColor(const Ray& ray, const Color& backgroundColor, const Hittable& world, int depth);

void createRandomScene(HittableList& world) {
	world.clear();
	auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto groundMaterial = make_shared<Lambertian>(checker);
	world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

	float sRadius = 0.2;
	for (int x = -11; x < 11; x++) {
		for (int z = -11; z < 11; z++) {
			auto mat = random_float();
			
			Point3 center(x + 0.9 * random_float(), sRadius, z + 0.9 * random_float());

			if ((center - Point3(4, 0.2, 0)).length() <= 0.9) continue;

			if (mat < 0.8) {
				Color albedo = Color(random_float(0.5, 1), random_float(0.5, 1), random_float(0.5, 1));
				Point3 center2 = center + Vec3(0, random_float(0, 0.5), 0);
				auto sphereMaterial = make_shared<Lambertian>(albedo);
				world.add(make_shared<MovingSphere>(center, center2, 0, 1, 0.2, sphereMaterial));
			}
			else if (mat < 0.85) {
				Color albedo = Color::random(0.5, 1);
				float fuzzy = random_float(0, 0.3);
				auto sphereMaterial = make_shared<Metal>(albedo, fuzzy);
				world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));	
			}
			else {
				auto sphereMaterial = make_shared<Dielectric>(1.5);
				world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
			}
		}
	}

	auto material1 = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

}

void twoPerlinSpheres(HittableList& world) {
	world.clear();

	auto pertext = make_shared<NoiseTexture>(4);
	world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
	world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));
}

void earth(HittableList& world) {
	world.clear();
	
	auto earth_texture = make_shared<ImageTexture>("E:\\Personal\\Blog\\RTNW\\img\\earthmap.jpg");
	auto earth_surface = make_shared<Lambertian>(earth_texture);
	auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

	world.add(globe);
}

void simpleLight(HittableList& world) {
	world.clear();

	auto pertext = make_shared<NoiseTexture>(4);
	world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
	world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

	auto difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
	world.add(make_shared<XYRect>(3, 5, 1, 3, -2, difflight));
}

void cornellBox(HittableList& world) {
	world.clear();
	auto red = make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

	world.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
	world.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
	world.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
	world.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
	world.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
	world.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

	shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
	world.add(box1);

	shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
	world.add(box2);
}

void cornellSmoke(HittableList& world) {

	auto red = make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

	world.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
	world.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
	world.add(make_shared<XZRect>(113, 443, 127, 432, 554, light));
	world.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
	world.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
	world.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

	shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));

	shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));

	world.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
	world.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));
}

void finalScene(HittableList& world) {
	world.clear();

	auto red = make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

	world.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
	world.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
	world.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
	world.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
	world.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
	world.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

	auto center1 = Point3(80, 50, 70);
	auto center2 = center1 + Vec3(0, 30, 0);
	auto moving_sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
	world.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	world.add(make_shared<Sphere>(Point3(400, 50, 90), 50, make_shared<Dielectric>(1.5)));

	world.add(make_shared<Sphere>(
		Point3(300, 50, 210), 70, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
		));

	auto boundary = make_shared<Sphere>(Point3(250, 50, 70), 50, make_shared<Dielectric>(1.5));
	world.add(boundary);
	world.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));

	auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("E:\\Personal\\Blog\\RTNW\\img\\earthmap.jpg"));
	world.add(make_shared<Sphere>(Point3(400, 100, 400), 100, emat));

	auto pertext = make_shared<NoiseTexture>(0.1);
	world.add(make_shared<Sphere>(Point3(150, 80, 350), 80, make_shared<Lambertian>(pertext)));

	//HittableList boxes2;
	//auto white = make_shared<Lambertian>(Color(.73, .73, .73));
	//int ns = 1000;
	//for (int j = 0; j < ns; j++) {
	//	boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
	//}

	//world.add(make_shared<Translate>(
	//	make_shared<RotateY>(
	//		make_shared<BVHNode>(boxes2, 0.0, 1.0), 15),
	//	Vec3(-100, 270, 395)
	//	)
	//);

}

int main()
{	
	float aspectRatio = 16.0 / 9.0;
	
	Point3 lookFrom(13, 2, 3);
	Point3 lookAt(0, 0, 0);
	Vec3 upVector(0, 1, 0);
	float focalDistance = 10.0;
	float aperture = 0;
	float vFOV = 20.0; // Vertical Field of View
	Color backgroundColor(0, 0, 0);
	
	// Image Size
	int imageWidth = 400;
	int samplesPerPixel = 100;
	int maxDepth = 50;

	// World space
	HittableList world;

	switch (0)
	{
	case 1:
		createRandomScene(world);
		backgroundColor = Color(0.70, 0.80, 1.00);
		lookFrom = Point3(13, 2, 3);
		lookAt = Point3(0, 0, 0);
		vFOV = 20.0;
		aperture = 0.01;
		break;
	case 2:
		twoPerlinSpheres(world);
		backgroundColor = Color(0.70, 0.80, 1.00);
		lookFrom = Point3(13, 2, 3);
		lookAt = Point3(0, 0, 0);
		vFOV = 20.0;
		aperture = 0.01;
		break;
	case 3:
		earth(world);
		backgroundColor = Color(0.70, 0.80, 1.00);
		lookFrom = Point3(13, 2, 3);
		lookAt = Point3(0, 0, 0);
		vFOV = 20.0;
		aperture = 0.01;
		break;
	case 4:
		simpleLight(world);
		backgroundColor = Color(0, 0, 0);
		//samplesPerPixel = 400;
		lookFrom = Point3(26, 3, 6);
		lookAt = Point3(0, 2, 0);
		vFOV = 20.0;
		break;
	case 5:
		cornellBox(world);
		aspectRatio = 1.0;
		imageWidth = 400;
		samplesPerPixel = 200;
		backgroundColor = Color(0, 0, 0);
		lookFrom = Point3(278, 278, -800);
		lookAt = Point3(278, 278, 0);
		vFOV = 40.0;
		break;
	case 6:
		cornellSmoke(world);
		aspectRatio = 1.0;
		imageWidth = 200;
		samplesPerPixel = 100;
		lookFrom = Point3(278, 278, -800);
		lookAt = Point3(278, 278, 0);
		vFOV = 40.0;
		break;
	default:
	case 8:
		finalScene(world);
		aspectRatio = 1.0;
		imageWidth = 200;
		samplesPerPixel = 100;
		lookFrom = Point3(278, 278, -800);
		lookAt = Point3(278, 278, 0);
		vFOV = 40.0;
		break;
	}
	int imageHeight = (int)(imageWidth / aspectRatio);

	Camera camera(lookFrom, lookAt, upVector, vFOV, aspectRatio, focalDistance, aperture, 0, 1);

	ofstream imageFile("image.ppm");
	
	imageFile << "P3\n" << imageWidth << " " << imageHeight << "\n" << "255\n" << endl;
	int kkk = 0;
	for (int h = 0; h < imageHeight; h++) {
		cout << "\rScanlines remaining: " << imageHeight - h << " " << flush;
		for (int w = 0; w < imageWidth; w++) {
			
			Color pixelColor;
			for (int ii = 0; ii < samplesPerPixel; ii++) {
				float v = 1 - (h + random_float()) / (imageHeight - 1.0);
				float u = (w +random_float()) / (imageWidth - 1.0);

				Ray ray = camera.getRay(u, v);
				pixelColor += rayColor(ray, backgroundColor, world, maxDepth);	
			}
		
			writeColor(imageFile, pixelColor, samplesPerPixel);
		}
	}
	cout << "\nDone.\n";
	imageFile.close();

	return 0;
}

Color rayColor(const Ray& ray, const Color& backgroundColor, const Hittable& world, int depth) {
	if (depth <= 0) return Color(0, 0, 0);
	
	if (Vec3::isNan(ray.direction()) || Point3::isNan(ray.origin()))
		return Color(0, 0, 0);

	HitRecord hitRecord;
	if (!world.hit(ray, 0.001, INF, hitRecord))
		return backgroundColor;

	Ray reflectedRay;
	Color attenuation;
	Color emitted = hitRecord.materialPtr->emitted(hitRecord.u, hitRecord.v, hitRecord.p);

	if (!hitRecord.materialPtr->scatter(ray, hitRecord, attenuation, reflectedRay)) {
		return emitted;
	}

	return emitted + attenuation * rayColor(reflectedRay, backgroundColor, world, depth - 1);
}
