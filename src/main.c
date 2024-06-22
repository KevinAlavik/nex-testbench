#include "framebuffer.h"
#include "nighterm.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

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

  framebuffer_t *fb = initialize_framebuffer(window);
  if (fb == NULL) {
    fprintf(stderr, "Could not initialize framebuffer\n");
    return 1;
  }

  struct nighterm_ctx context;
  nighterm_initialize(&context, NULL, (void *)(uint64_t)fb->buffer, fb->width,
                      fb->height, fb->bpp, malloc, free);

  nighterm_flush(&context, 60, 55, 255);

  int quit = 0;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }

    for (int y = 0; y < fb->height; y++) {
      for (int x = 0; x < fb->width; x++) {
        uint32_t pixel = ((uint32_t *)fb->buffer)[x * (fb->bpp >> 3) + y * fb->pitch];
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