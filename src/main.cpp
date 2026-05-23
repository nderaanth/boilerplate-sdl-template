#include <cstdio>
#include <cstdlib>
#include <SDL3/SDL.h>

#include "bgr.hpp"
#include "types.hpp"
// yes i created a custom lib for some obscute type

using namespace Types;
int main(void) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		// im  a lazy little guy who didn't bother making the error codes
		return 1;
	}

	Types::UVector2 window_dimensions = {1024, 768};
	SDL_Window *window = SDL_CreateWindow("IndustrialGradeCoolant", window_dimensions.x, 
					      window_dimensions.y, SDL_WINDOW_NOT_FOCUSABLE);

	if (!window) {
		SDL_Log("Failed to create window %s", SDL_GetError());
		// TODO: create a standard for err codes
		SDL_Quit();
		exit(2);
	}

	SDL_Surface *surface = SDL_GetWindowSurface(window);
	Graphics::Framebuffer  fb; 

	// Populate the framebuffer object
	fb.address = (uint32_t *)surface->pixels;

	// since we use uint32_t for the address, we'll divide the pitch by 4 because surface->pitch is in bytes
	fb.pitch = surface->pitch / 4;
	fb.target_framerate = 60;
	fb.width = window_dimensions.x;
	fb.height = window_dimensions.y;

	// Fetch the BPP
	{
		// We don't objectively *need* details here, but it's nice to not have it polluting the stack
		const SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(surface->format);
		if (details) {
			fb.bpp = details->bytes_per_pixel;
		}
	}

	static bool is_running = true;
	SDL_Event event;
	

	/// Timing blocks 

	uint64_t	start_tick_ns = 0;
	uint64_t	postactive_tick_ns = 0;
	float 		frametime_ns = 0;
	
	const uint64_t TARGET_FRAMETIME_MS = 1000000000 / fb.target_framerate;
	float		framerate  = 0;
	float 		delta_time_ns = 0;

	while (is_running) {
		start_tick_ns = SDL_GetTicksNS();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT: 
					is_running = false;
					break;
			}
		}
		// start drawing here

		Bgr::Plot(fb, 0, 0, 0x0ff00ff);
		for (int i = 0; i < 767; i++) {
			Bgr::Plot(fb, 1, i, 0x00ff00);
		}
		fb.address[1024] = 0xff00ff;


		SDL_UpdateWindowSurface(window);

		postactive_tick_ns = SDL_GetTicksNS();
		frametime_ns = postactive_tick_ns - start_tick_ns;
		
		// sleep to control framerate exactly
		if  (frametime_ns < TARGET_FRAMETIME_MS) {
			SDL_DelayNS(TARGET_FRAMETIME_MS - frametime_ns);
		}

		frametime_ns = SDL_GetTicksNS() - start_tick_ns;

		delta_time_ns = (float)frametime_ns / 1000000000.0f;
		framerate = (delta_time_ns > 0.0f) ?
			(1.0f / delta_time_ns) : 0.0f;


		std::printf("FPS: %.2f\r", framerate);
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
