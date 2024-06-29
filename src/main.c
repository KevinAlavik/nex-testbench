#include "framebuffer.h"
#include "nighterm.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

void putpixel(void *framebuffer, uint64_t bpp, uint64_t pitch, int x, int y,
              uint32_t color) {
  uint32_t *pixel = (uint32_t *)framebuffer + ((x * bpp >> 3) + y * pitch);
  *pixel = color;
}

void draw_rect(void *framebuffer, uint64_t bpp, uint64_t pitch, int x, int y,
               int w, int h, uint32_t color) {
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      putpixel(framebuffer, bpp, pitch, x + j, y + i, color);
    }
  }
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Nighterm Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,
      HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
    return 1;
  }

  framebuffer_t *fb = initialize_framebuffer(WIDTH, HEIGHT);
  if (fb == NULL) {
    fprintf(stderr, "Could not initialize framebuffer\n");
    return 1;
  }

  struct nighterm_ctx context;
  int init_status = nighterm_initialize(&context, NULL, fb->buffer, WIDTH,
                                        HEIGHT, 32, malloc, free);
  if (init_status != NIGHTERM_SUCCESS) {
    fprintf(stderr, "Nighterm initialization failed\n");
    destroy_framebuffer(fb);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  draw_rect(fb->buffer, 32, fb->pitch, 10, 10, 100, 100, 0xFF0000FF);

  update_framebuffer_texture(renderer, NULL, fb);

  int quit = 0;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }

    // Draw pixels directly to the renderer
    for (int y = 0; y < fb->height; y++) {
      for (int x = 0; x < fb->width; x++) {
        uint32_t pixel = ((uint32_t *)fb->buffer)[x + y * fb->width];
        uint8_t a = (pixel >> 24) & 0xff;
        uint8_t r = (pixel >> 16) & 0xff;
        uint8_t g = (pixel >> 8) & 0xff;
        uint8_t b = (pixel >> 0) & 0xff;

        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}