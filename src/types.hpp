#pragma once

#include <cstdint>

namespace Types {
	struct UVector2 {
		uint32_t x, y;
	};

	struct Vector2 {
		int32_t x, y;
	};
}

namespace Types::Graphics {
	struct Framebuffer {
		uint32_t *address;
		uint32_t width;
		uint32_t height;
		uint32_t pitch;
		uint32_t bpp;

		uint32_t target_framerate;
	};
}
