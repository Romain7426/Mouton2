#ifndef SDL_H
#define SDL_H

extern bool sdl_init(void);
extern void sdl_end(void);

extern void sdl_pixel_format_print(const SDL_PixelFormat * format);

extern SDL_PixelFormat sdl_pixel_format_rgb;
extern SDL_PixelFormat sdl_pixel_format_rgba;

extern const Uint32 r32mask;
extern const Uint32 g32mask;
extern const Uint32 b32mask;
extern const Uint32 a32mask;

extern const Uint32 r24mask;
extern const Uint32 g24mask;
extern const Uint32 b24mask;
extern const Uint32 a24mask;


#endif /* SDL_H */
