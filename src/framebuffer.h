#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct {
  uint32_t *buffer;
  uint64_t width;
  uint64_t height;
  uint64_t pitch;
} framebuffer_t;

framebuffer_t *initialize_framebuffer(uint64_t width, uint64_t height);
void update_framebuffer_texture(SDL_Renderer *renderer, SDL_Texture *texture,
                                framebuffer_t *fb);
void destroy_framebuffer(framebuffer_t *fb);

#endif // FRAMEBUFFER_H
