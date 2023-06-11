#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "RTNW.h"
#include "Color.h"
#include "Vec3.h"
#include "Perlin.h"
#include "rtnw_stb_image.h"

class Texture {
public:
	virtual Color value(float u, float v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
public:
	SolidColor() {};
	SolidColor(Color c) : colorValue(c) {};
	SolidColor(float red, float green, float blue) : colorValue(Color(red, green, blue)) {};
	
	virtual Color value(float u, float v, const Point3& p) const override {
		return colorValue;
	}
private:
	Color colorValue;
};

class CheckerTexture : public Texture {
public:
	CheckerTexture() {};
	CheckerTexture(shared_ptr<Texture> e, shared_ptr<Texture> o) 
		: even(e), odd(o) {};
	CheckerTexture(Color c1, Color c2) 
		: even(make_shared<SolidColor>(c1)), odd(make_shared<SolidColor>(c2)) {}
	
	virtual Color value(float u, float v, const Point3& p) const override {
		float sines = sinf(10 * p.x()) * sinf(10 * p.y()) * sinf(10 * p.z());
		if (sines < 0) return odd->value(u, v, p);
		return even->value(u, v, p);
	}
private:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
};

class NoiseTexture : public Texture {
public:
	NoiseTexture() : scale(1) {}
	NoiseTexture(float s) : scale(s) {}
	virtual Color value(float u, float v, const Point3& p) const override {
		return Color(1, 1, 1) * 0.5 * (1 + sinf(scale * p.z() + 10 * noise.turb(scale * p)));
	}
private:
	Perlin noise;
	float scale;
};

class ImageTexture : public Texture {
public:
	const static int bytes_per_pixel = 3;

	ImageTexture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {};

	ImageTexture(const char* fileName) {
		auto components_per_pixel = bytes_per_pixel;
		data = stbi_load(fileName, &width, &height, &components_per_pixel, components_per_pixel);

		if (data == nullptr) {
			std::cerr << "ERROR: Could not load texture image file '" << fileName << "'.\n";
			width = height = 0;
		}

		bytes_per_scanline = bytes_per_pixel * width;
	}

	~ImageTexture() { delete data; }

	virtual Color value(float u, float v, const Point3& p) const override {

		// If we have no texture data, then return solid cyan as a debugging aid.
		if (data == nullptr) {
			return Color(0, 1, 1);
		}

		// Clamp input texture coordinates to [0,1] x [1,0]
		u = clamp(u, 0, 1);
		v = 1 - clamp(v, 0, 1); // Flip V to image coordinates

		int i = int(u * width);
		int j = int(v * height);

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i >= width)  i = width - 1;
		if (j >= height) j = height - 1;

		float color_scale = 1.0 / 255.0;
		unsigned char* pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

		return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}

private:
	unsigned char* data;
	int width, height;
	int bytes_per_scanline;
};

#endif // !TEXTURE_H
