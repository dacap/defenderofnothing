#ifndef GFX_HPP
#define GFX_HPP


// pixels of the screen
#define GFX_W		360
#define GFX_H		240
#define GFX_CX		(GFX_W/2)
#define GFX_CY		(GFX_H/2)


bool setup_gfx();

// void create_color_tables();
void make_screenshot(BITMAP *bmp);
void trans_mode(int a);
void lit_mode(int r, int g, int b);


#endif // GFX_HPP
