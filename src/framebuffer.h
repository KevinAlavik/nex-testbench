#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct {
    uint32_t *buffer;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
} framebuffer_t;

framebuffer_t* initialize_framebuffer(SDL_Window* window);

#endif // FRAMEBUFFER_H