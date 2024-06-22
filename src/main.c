
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
  SDL_Texture *texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, fb->width, fb->height);

  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }

    SDL_UpdateTexture(texture, NULL, fb->buffer, fb->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}