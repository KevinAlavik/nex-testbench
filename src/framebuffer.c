#include "framebuffer.h"
#include <stdlib.h>

framebuffer_t *initialize_framebuffer(uint64_t width, uint64_t height) {
  framebuffer_t *framebuffer = (framebuffer_t *)malloc(sizeof(framebuffer_t));
  if (!framebuffer) {
    return NULL;
  }

  framebuffer->width = width;
  framebuffer->height = height;
  framebuffer->pitch = width * sizeof(uint32_t);
  framebuffer->buffer = (uint32_t *)malloc(height * framebuffer->pitch);

  if (!framebuffer->buffer) {
    free(framebuffer);
    return NULL;
  }

  return framebuffer;
}

void update_framebuffer_texture(SDL_Renderer *renderer, SDL_Texture *texture,
                                framebuffer_t *fb) {
  if (!texture) {
    texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                          SDL_TEXTUREACCESS_STREAMING, fb->width, fb->height);
  }

  SDL_UpdateTexture(texture, NULL, fb->buffer, (int)fb->pitch);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void destroy_framebuffer(framebuffer_t *fb) {
  if (fb) {
    if (fb->buffer) {
      free(fb->buffer);
    }
    free(fb);
  }
}
