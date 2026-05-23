#pragma once

#include <cstdint>
#include "types.hpp"

namespace Bgr {
	
void Plot(Types::Graphics::Framebuffer &fb,
	       uint32_t x, 
	       uint32_t y, 
	       uint32_t color
);


};
