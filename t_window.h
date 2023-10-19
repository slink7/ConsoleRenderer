
#ifndef T_WINDOW_H
#define T_WINDOW_H

typedef struct s_vec2i
{
	int x;
	int y;
}	t_vec2i;

typedef struct s_vec2
{
	float x;
	float y;
}	t_vec2;

typedef struct s_window
{
	char *palette;
	int width;
	int height;
	char *buffer;
}	t_window;

t_window	*CR_new_window(int w, int h);
void		CR_clear(t_window *w, char c);
void		CR_present(t_window *w);
void		CR_write(t_window *w, t_vec2i *p, char *str);
void		CR_writef(t_window *w, t_vec2 *p, char *str);
void		CR_write_centered(t_window *w, t_vec2i *p, char *str);
void		CR_write_centeredf(t_window *w, t_vec2 *p, char *str);
void		CR_draw(t_window *w, t_vec2i *p, float v);
void		CR_drawf(t_window *w, t_vec2 *p, float v);
void		CR_draw_line(t_window *w, t_vec2i *p0, t_vec2i *p1, float v);
void		CR_draw_linef(t_window *w, t_vec2 *p0, t_vec2 *p1, float v);
void		CR_draw_triangle(t_window *w, t_vec2i *p0, t_vec2i *p1, t_vec2i *p2, float v);
void		CR_draw_trianglef(t_window *w, t_vec2 *p0, t_vec2 *p1, t_vec2 *p2, float v);

#endif