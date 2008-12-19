#ifndef GFX_HPP
#define GFX_HPP


// pixels of the screen
#define GFX_W		_gfx_w	// 360 or 320
#define GFX_H		240
#define GFX_CX		(GFX_W/2)
#define GFX_CY		(GFX_H/2)


extern int _gfx_w;


bool setup_gfx(bool widescreen);
void switch_gfx_mode();

void make_screenshot(BITMAP *bmp);
void trans_mode(int a);
void lit_mode(int r, int g, int b);

int blend_color(int from, int to, double t);

void draw_text(BITMAP *bmp, int x, int y, int color, const char *format, ...);


#endif // GFX_HPP
