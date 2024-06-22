#include "framebuffer.h"

framebuffer_t *initialize_framebuffer(SDL_Window *window) {
  framebuffer_t *framebuffer = (framebuffer_t *)malloc(sizeof(framebuffer_t));
  if (framebuffer == NULL) {
    return NULL;
  }

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  framebuffer->width = surface->w;
  framebuffer->height = surface->h;
  framebuffer->bpp = surface->format->BitsPerPixel;

  int pitch = framebuffer->width * ((framebuffer->bpp | 7) >> 3);
  framebuffer->pitch = pitch;
  framebuffer->buffer = (uint32_t *)malloc(
      sizeof(uint32_t) * (framebuffer->width * (framebuffer->bpp >> 3) +
                          framebuffer->height * framebuffer->pitch));
  if (framebuffer->buffer == NULL) {
    free(framebuffer);
    return NULL;
  }

  return framebuffer;
}