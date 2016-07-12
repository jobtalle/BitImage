#ifndef BIT_IMAGE
#define BIT_IMAGE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum bit_image_format {
	BIT_IMAGE_FORMAT_RGB,
	BIT_IMAGE_FORMAT_RGBA,
	BIT_IMAGE_FORMAT_GRAYSCALE
};

struct bit_image_pixel_rgb {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct bit_image_pixel_rgba {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct bit_image_pixel_grayscale {
	uint8_t v;
};

struct bit_image_pixel_general {
	union {
		struct bit_image_pixel_rgb rgb;
		struct bit_image_pixel_rgba rgba;
		struct bit_image_pixel_grayscale grayscale;
	};
};

static uint8_t bit_image_stride(const enum bit_image_format format) {
	switch(format) {
		case BIT_IMAGE_FORMAT_RGB:
			return sizeof(struct bit_image_pixel_rgb);
			break;
		case BIT_IMAGE_FORMAT_RGBA:
			return sizeof(struct bit_image_pixel_rgba);
			break;
		case BIT_IMAGE_FORMAT_GRAYSCALE:
			return sizeof(struct bit_image_pixel_grayscale);
			break;
	}
	
	return 0;
}

static uint8_t *bit_image_write(
	const uint8_t *source,
	const uint32_t width,
	const uint32_t height,
	const struct bit_image_pixel_general color,
	const enum bit_image_format format) {
	uint32_t i;
	uint32_t pixel_count = width * height;
	uint8_t stride;
	uint8_t *result = NULL;
	
	stride = bit_image_stride(format);
	result = (uint8_t*)calloc(pixel_count >> 3, 1);
	
	for(i = 0; i < pixel_count; ++i) {
		uint8_t byte = i >> 3;
		uint8_t bit = i - (byte << 3);
		void *pixel = (void*)(source + stride * i);
		
		if(memcmp(&color, pixel, stride) == 0) {
			result[byte] |= 1 << (i - (byte << 3));
		}
	}
	
	return result;
}

static uint8_t *bit_image_read(
	const uint8_t *source,
	const uint32_t width,
	const uint32_t height,
	const struct bit_image_pixel_general color_zero,
	const struct bit_image_pixel_general color_one,
	const enum bit_image_format format) {
	uint32_t i;
	uint32_t pixel_count = width * height;
	uint8_t stride;
	uint8_t *result = NULL;
	
	stride = bit_image_stride(format);
	result = (uint8_t*)malloc(pixel_count * stride);
	
	for(i = 0; i < pixel_count; ++i) {
		uint8_t byte = i >> 3;
		uint8_t bit = i - (byte << 3);
		void *pixel = result + stride * i;
		
		if(source[byte] & (1 << bit)) {
			memcpy(pixel, &color_one, stride);
		}
		else {
			memcpy(pixel, &color_zero, stride);
		}
	}
	
	return result;
}

#ifdef __cplusplus
}
#endif

#endif