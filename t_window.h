
#ifndef T_WINDOW_H
#define T_WINDOW_H

typedef struct s_window
{
	char *palette;
	int width;
	int height;
	char *buffer;
}	t_window;

t_window	*CR_new_window(int w, int h);
void		CR_set_cursor_pos(int x, int y);
void		CR_clear(t_window *w, char c);
void		CR_present(t_window *w);
void		CR_write(t_window *w, int x, int y, char *str);
void		CR_writef(t_window *w, float x, float y, char *str);
void		CR_draw(t_window *w, int x, int y, float v);
void		CR_drawf(t_window *w, float x, float y, float v);
void		CR_draw_line(t_window *w, int x0, int y0, int x1, int y1, float v);
void		CR_draw_linef(t_window *w, float x0, float y0, float x1, float y1, float v);
void		CR_draw_triangle(t_window *w, int x0, int y0, int x1, int y1, int x2, int y2, float v);
void		CR_draw_trianglef(t_window *w, float x0, float y0, float x1, float y1, float x2, float y2, float v);
int			CR_unnormalize(t_window *w, float x, char c);

#endif