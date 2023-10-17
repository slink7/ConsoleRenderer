#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "t_window.h"

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int min(int a, int b) {
	return (a < b) ? a : b;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

void	CR_set_cursor_pos(int x, int y)
{
	printf("\033[%d;%dH", y, x);
}

int CR_unnormalize(t_window *w, float x, char c) {
	return (int) round((x / 2.0f + 0.5f) * ((c == 'h') ? w->height : w->width));
}

t_window	*CR_new_window(int w, int h)
{
	t_window *out = malloc(sizeof(t_window));
	out->palette = "@#W$9876543210?!abc;:+=-,._ "; //ertdfgcvb
	out->width = w;
	out->height = h;
	out->buffer = malloc(sizeof(char) * (out->width + 1) * (out->height));
	CR_clear(out, '.');
	return out;
}

void	CR_clear(t_window *w, char c)
{
	for (int k = 0; k < (w->width + 1) * (w->height); k++)
		w->buffer[k] = (k % (w->width + 1) == w->width) ? '\n' : c;
}

void	CR_present(t_window *w)
{
	printf("\e[1;1H\e[2J");
	printf("%s", w->buffer);
}

void	CR_write(t_window *w, int x, int y, char *str)
{
	for (int k = 0; str[k] && k + x < w->width; k++)
		w->buffer[y * (w->width + 1) + x + k] = str[k];
}

void	CR_writef(t_window *w, float x, float y, char *str)
{
	CR_write(w, CR_unnormalize(w, x, 'w'), CR_unnormalize(w, y, 'h'), str);
}

void	CR_draw(t_window *w, int x, int y, float v)
{
	if (x < 0 || x >= w->width || y < 0 || y >= w->height)
		return;
	int index = (int) round((1.0f - v) * ((float) strlen(w->palette) - 1.0f));
	w->buffer[y * (w->width + 1) + x] = w->palette[index];
}

void	CR_drawf(t_window *w, float x, float y, float v)
{
	CR_draw(w, CR_unnormalize(w, x, 'w'), CR_unnormalize(w, y, 'h'), v);
}

void	CR_draw_line(t_window *w, int x0, int y0, int x1, int y1, float v)
{
	int dx = x1 - x0; //64
	int dy = y1 - y0; // 0
	if (abs(dx) > abs(dy)) {
		float a = (float) dy / (float) dx; //0
		float b = - a * (float) x0 + (float) y0; 
		for (int k = min(x0, x1); k <= max(x0, x1); k++) {
			CR_draw(w, k,(int) round(a * (float) k + b), v);
		}
	} else {
		float a = (float) dx / (float) dy;
		float b = - a * (float) y0 + (float) x0;
		for (int k = min(y0, y1); k <= max(y0, y1); k++) {
			//printf("llop %d %d %d %d\n", y0, y1, max(y0, y1), k);
			CR_draw(w, (int) round(a * (float) k + b), k, v);
		}
	}
}

void	CR_draw_linef(t_window *w, float x0, float y0, float x1, float y1, float v)
{
	CR_draw_line(w, CR_unnormalize(w, x0, 'w'), CR_unnormalize(w, y0, 'h'), CR_unnormalize(w, x1, 'w'), CR_unnormalize(w, y1, 'h'), v);
}

void	CR_draw_triangle(t_window *w, int x0, int y0, int x1, int y1, int x2, int y2, float v)
{
	CR_draw_line(w, x0, y0, x1, y1, v);
	CR_draw_line(w, x1, y1, x2, y2, v);
	CR_draw_line(w, x2, y2, x0, y0, v);
}

void	CR_draw_trianglef(t_window *w, float x0, float y0, float x1, float y1, float x2, float y2, float v)
{
	CR_draw_triangle(w, CR_unnormalize(w, x0, 'w'), CR_unnormalize(w, y0, 'h'), CR_unnormalize(w, x1, 'w'), CR_unnormalize(w, y1, 'h'), CR_unnormalize(w, x2, 'w'), CR_unnormalize(w, y2, 'h'), v);
}