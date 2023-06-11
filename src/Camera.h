#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "RTNW.h"
#include "Ray.h"

class Camera {
public:
	
	Camera(
		Point3 lookFrom,
		Point3 lookAt,
		Vec3 upVector,
		float vPoV, 
		float aspectRatio,
		float focalDistance,
		float aperture,
		float _time0,
		float _time1
	) {
		//  vPoV: Vertical Point of View
		float theta = degrees2radian(vPoV);
		float h = tanf(theta / 2) * focalDistance;
		
		// Sensor size
		float viewPortHeight = 2.0 * h;
		float viewPortWidth = viewPortHeight * aspectRatio;

		// Camera Coordinate

		zVector = unitVector(lookFrom - lookAt);
		xVector = unitVector(cross(upVector, zVector));
		yVector = cross(zVector, xVector);

		origin = lookFrom;
		vertical = viewPortHeight * yVector;
		horizontal = viewPortWidth * xVector;
		bottomLeftCorner = origin - vertical / 2 - horizontal / 2 - focalDistance * zVector;

		lensRadius = aperture / 2;

		time0 = _time0;
		time1 = _time1;
	}

	Ray getRay(float u, float v) {
		// Cast a ray from the camera through a pixel point in the image plane.

		// Defocus blur
		Vec3 noise = lensRadius * randomInUnitDisk();
		Vec3 offset = noise.x() * xVector + noise.y() * yVector;

		Point3 pixelPoint = u * horizontal + v * vertical + bottomLeftCorner;
		Vec3 direction = (pixelPoint + offset) - origin;
		float shootingTime = random_float(time0, time1);

		Ray ray(origin, direction, shootingTime);

		return ray;
	}
private:
	
	// Sensor size
	float lensRadius;
	Vec3 zVector;
	Vec3 xVector;
	Vec3 yVector;

	// Camera's Position
	Point3 origin;
	Vec3 vertical;
	Vec3 horizontal;
	Point3 bottomLeftCorner;

	// Shutter open/ close time
	float time0, time1;
};

#endif // !CAMERA_H
