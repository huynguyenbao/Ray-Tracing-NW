// RT1W.cpp : Defines the entry point for the application.
//

#include "RT1W.h"
#include "Camera.h"
#include "Ray.h"
#include "HittableList.h"
#include "Color.h"
#include "Sphere.h"
#include "Material.h"

#include <iostream>
#include <fstream>


using namespace std;

Color rayColor(const Ray& ray, const Hittable& world, int maxDepth);

void createRandomScene(HittableList& world) {
	world.clear();
	
	auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

	float sRadius = 0.2;
	for (int x = -3; x < 3; x++) {
		for (int z = -3; z < 3; z++) {
			auto mat = random_float();
			
			Point3 center(x + 0.9 * random_float(), sRadius, z + 0.9 * random_float());

			if ((center - Point3(4, 0.2, 0)).length() <= 0.9) continue;

			if (mat < 0.8) {
				Color albedo = Color(random_float(0.5, 1), random_float(0.5, 1), random_float(0.5, 1));
				auto sphereMaterial = make_shared<Lambertian>(albedo);
				world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
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

int main()
{
	float aspectRatio = 3.0 / 2.0;
	
	Point3 lookFrom(9, 2, 11);
	Point3 lookAt(0, 0, 0);
	Vec3 upVector(0, 1, 0);
	float focalDistance = 10.0;
	float aperture = 0.1;

	Camera camera(lookFrom, lookAt, upVector, 20, aspectRatio, focalDistance, aperture);
	
	// Image Size
	const int imageWidth = 1200;
	const int imageHeight = (int) (imageWidth / aspectRatio);
	const int samplesPerPixel = 500;
	const int maxDepth = 50;

	// World space
	HittableList world;
	
	createRandomScene(world);

	ofstream imageFile("image.ppm");
	
	imageFile << "P3\n" << imageWidth << " " << imageHeight << "\n" << "255\n" << endl;

	for (int h = 0; h < imageHeight; h++) {
		cout << "\rScanlines remaining: " << imageHeight - h << " " << flush;
		for (int w = 0; w < imageWidth; w++) {
			
			Color pixelColor;
			for (int ii = 0; ii < samplesPerPixel; ii++) {
				float v = 1 - (h + random_float()) / (imageHeight - 1.0);
				float u = (w +random_float()) / (imageWidth - 1.0);

				Ray ray = camera.getRay(u, v);

				pixelColor += rayColor(ray, world, maxDepth);	
			}
		
			writeColor(imageFile, pixelColor, samplesPerPixel);
		}
	}
	cout << "\nDone.\n";
	imageFile.close();

	return 0;
}

Color rayColor(const Ray& ray, const Hittable& world, int depth) {
	if (depth <= 0) return Color(0, 0, 0);

	HitRecord hitRecord;
	if(world.hit(ray, 0.001, INF, hitRecord)) {
		Ray reflectedRay;
		Color attenuation;
		if (hitRecord.materialPtr->scatter(ray, hitRecord, attenuation, reflectedRay)) {
			return attenuation * rayColor(reflectedRay, world, depth - 1);
		}
		return Color(0, 0, 0);
	}
	
	Vec3 u = unitVector(ray.direction());
	float t = 0.5 * (u.y() + 1);
	return (1 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1.0);
}
