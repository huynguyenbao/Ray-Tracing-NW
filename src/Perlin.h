#pragma once
#ifndef PERLIN_H
#define PERLIN_H

#include "RTNW.h"
#include "Vec3.h"

class Perlin {
public:
	Perlin() {
		randVecs = new Vec3[pointCount];
		for (int i = 0; i < pointCount; i++) {
			randVecs[i] = unitVector(Vec3::random(-1, 1));
		}
		xPermutaion = perlinGeneratePerm();
		yPermutaion = perlinGeneratePerm();
		zPermutaion = perlinGeneratePerm();
	}

	~Perlin() {
		delete randVecs;
		delete xPermutaion;
		delete yPermutaion;
		delete zPermutaion;
	}

	float noise(const Point3& p) const {
		float u = p.x() - floorf(p.x());
		float v = p.y() - floorf(p.y());
		float w = p.z() - floorf(p.z());

		u = u * u * (3 - 2 * u);
		v = v * v * (3 - 2 * v);
		w = w * w * (3 - 2 * w);

		int ii = static_cast<int>(floor(p.x()));
		int jj = static_cast<int>(floor(p.y()));
		int kk = static_cast<int>(floor(p.z()));

		Vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] =
					randVecs[
						xPermutaion[(ii + di) & 255] ^
							yPermutaion[(jj + dj) & 255] ^
							zPermutaion[(kk + dk) & 255]];

		return triLerp(c, u, v, w);
	}

	float turb(const Point3& p, int depth = 1) const {
		float accum = 0;
		Vec3 tmp = p;
		float weight = 1;

		for (int ii = 0; ii < depth; ii++) {
			accum += weight * noise(tmp);
			weight *= 0.5;
			tmp *= 2;
		}
		
		return fabs(accum);
	}
private:
	static const int pointCount = 256;
	Vec3* randVecs;

	int* xPermutaion;
	int* yPermutaion;
	int* zPermutaion;

	static int* perlinGeneratePerm() {
		int* p = new int[pointCount];
		for (int i = 0; i < pointCount; i++) {
			p[i] = i;
		}
		permutate(p, pointCount);

		return p;
	}

	static void permutate(int* p, int N) {
		for (int i = N - 1; i > 0; i--) {
			int target = random_int(0, i);
			int tmp = p[target];
			p[target] = p[i];
			p[i] = tmp;
		}
	}

	static float triLerp(Vec3 c[2][2][2], float u, float v, float w) {
		float accum = 0;
		for (int ii = 0; ii < 2; ii++) {
			for (int jj = 0; jj < 2; jj++) {
				for (int kk = 0; kk < 2; kk++) {
					Vec3 weight(u - ii, v - jj, w - kk);
					accum += (ii * u + (1 - ii) * (1 - u)) *
						(jj * v + (1 - jj) * (1 - v)) *
						(kk * w + (1 - kk) * (1 - w)) * dot(c[ii][jj][kk], weight);
				}
			}
		}
		return accum;
	}
};

#endif // !PERLIN_H
