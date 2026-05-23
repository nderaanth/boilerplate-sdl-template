#include "types.hpp"	
#include "bgr.hpp"

// WARNING!!!: DO !!NOT!! use this in a loop because this is really slow if you manually do thousands of function calls in a loop,
// if you think my other algorythyms are slow, access the u32 ptr in 
// Types::Framebuffer. If you are even  more paranoid, manually use 
// the surface->pixels in SDL_Surface* that SDL_GetWindowSurface returns
// (though you still need a window object, obviously). If you still think 
// it's slow, it's no longer my problem, complain to the guys behind SDL.
//
// If you want to use my lib but don't trust this specific function, make
// your own if I didn't make one intended for repeated use.
//
// - Cache the variables that don't change like  the address or width (if you'll actually use the  vars!)
// - Write to the address but beware of bounds.
void Bgr::Plot(Types::Graphics::Framebuffer &fb,
	       uint32_t x, 
	       uint32_t y, 
	       uint32_t color
) {
	if (x >= fb.width || y >= fb.height) [[unlikely]]
		return;

	fb.address[y * fb.pitch + x ] = color;
}
