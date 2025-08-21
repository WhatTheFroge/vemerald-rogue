#ifndef GUARD_RGB_H
#define GUARD_RGB_H

#define GET_R(color) ((color) & 0x1F)
#define GET_G(color) (((color) >> 5) & 0x1F)
#define GET_B(color) (((color) >> 10) & 0x1F)

#define RGB(r, g, b)  ((r) | ((g) << 5) | ((b) << 10))
#define RGB2(r, g, b) (((b) << 10) | ((g) << 5) | (r))
#define _RGB(r, g, b) ((((b) & 0x1F) << 10) + (((g) & 0x1F) << 5) + ((r) & 0x1F))

#define RGB_ALPHA       (1 << 15)
#define IS_ALPHA(color) ((color) & RGB_ALPHA)

#define RGB_BLACK      RGB(0, 0, 0)
#define RGB_WHITE      RGB(31, 31, 31)
#define RGB_RED        RGB(31, 0, 0)
//#define RGB_GREEN		RGB(31, 15, 0) 	// green is actually orange 
#define RGB_GREEN      RGB(0, 31, 0)
#define RGB_BLUE       RGB(0, 0, 31)
#define RGB_YELLOW     RGB(31, 31, 0)
#define RGB_MAGENTA    RGB(31, 0, 31)
#define RGB_CYAN       RGB(0, 31, 31)
#define RGB_WHITEALPHA (RGB_WHITE | RGB_ALPHA)

// new 
#define RGB_ORANGE 		RGB(31, 16, 0)  // bright orange: max red, medium green, no blue


#endif // GUARD_RGB_H
