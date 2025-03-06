#ifndef OPENGL2D_H
#define OPENGL2D_H

extern void begin2d(void);
extern void end2d(void);
extern void blitter2d(int x1, int y1, int x2, int y2);
extern void blitter2d_via_centre(real cx, real cy, real h);

#endif /* OPENGL2D_H */
