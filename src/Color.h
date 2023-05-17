#pragma once

#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
typedef Vec3 Color;

using namespace std;

inline float clamp(float x, float xMin, float xMax) {
	if (x < xMin) x = xMin;
	if (x > xMax) x = xMax;
	return x;
}

void writeColor(ostream& out, Color pixelColor, int samplesPerPixel) {
	float r = clamp(pixelColor.x() / samplesPerPixel, 0, 1);
	float g = clamp(pixelColor.y() / samplesPerPixel, 0, 1);
	float b = clamp(pixelColor.z() / samplesPerPixel, 0, 1);

	// Gamma Correction
	r = sqrtf(r);
	g = sqrtf(g);
	b = sqrtf(b);

	out << (int) (255.999 * r) << " " <<
		(int) (255.999 * g) << " " <<
		(int) (255.999 * b) << " " << endl;
}

#endif // !COLOR_H
