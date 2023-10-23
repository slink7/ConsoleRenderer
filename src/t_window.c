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

void	normalize(t_window *w, t_vec2i *v, t_vec2 *out)
{
	out->x = ((float) v->x / (float) w->width) * 2.0f - 1.0f;
	out->y = ((float) v->y / (float) w->height) * 2.0f - 1.0f;
}

void	unnormalize(t_window *w, t_vec2 *v, t_vec2i *out) {
	out->x = (int) round((v->x + 0.5f) * (float) w->width);
	out->y = (int) round((v->y / 2.0f + 0.5f) * (float) w->height);
}

int		pos_to_index(t_window *w, t_vec2i *p)
{
	return p->y * (w->width + 1) + p->x;
}

t_window	*CR_new_window(int w, int h)
{
	t_window *out = malloc(sizeof(t_window));
	out->palette = "@$089W#b654321ac?!;:+=_-,. "; //ertdfgcvb
	out->width = w;
	out->height = h;
	out->buffer = malloc(sizeof(char) * (out->width + 1) * (out->height));
	CR_clear(out, '.');
	return out;
}

void	CR_free_window(t_window **w)
{
	free((*w)->buffer);
	free(*w);
	*w = 0;
}

void	CR_clear(t_window *w, char c)
{
	for (int k = 0; k < (w->width + 1) * (w->height); k++)
		w->buffer[k] = (k % (w->width + 1) == w->width) ? '\n' : c;
}

void	CR_present(t_window *w)
{
	write(1, "\e[1;1H\e[2J", 10);
	write(1, w->buffer, (w->width + 1) * w->height);
}

void	CR_write(t_window *w, t_vec2i *p, char *str)
{
	for (int k = 0; str[k] && k + p->x < w->width; k++)
		w->buffer[pos_to_index(w, p) + k] = str[k];
}

void	CR_writef(t_window *w, t_vec2 *p, char *str)
{
	t_vec2i pi;
	unnormalize(w, p, &pi);
	CR_write(w, &pi, str);
}

int ft_strlen(char *str)
{
	int k;

	k = 0;
	while (*(str + k))
		k++;
	return (k);
}

void	CR_write_centered(t_window *w, t_vec2i *p, char *str)
{
	int len = ft_strlen(str);
	for (int k = 0; str[k] && k + p->x < w->width; k++)
		w->buffer[pos_to_index(w, p) + k - len / 2] = str[k];
}

void	CR_write_centeredf(t_window *w, t_vec2 *p, char *str)
{
	t_vec2i pi;
	unnormalize(w, p, &pi);
	CR_write_centered(w, &pi, str);
}

void	CR_draw(t_window *w, t_vec2i *p, float v)
{
	if (p->x < 0 || p->x >= w->width || p->y < 0 || p->y >= w->height)
		return;
	int index = (int) round((1.0f - v) * ((float) strlen(w->palette) - 1.0f));
	w->buffer[pos_to_index(w, p)] = w->palette[index];
}

void	CR_drawf(t_window *w, t_vec2 *p, float v)
{
	t_vec2i pi;
	unnormalize(w, p, &pi);
	CR_draw(w, &pi, v);
}

void	CR_draw_line(t_window *w, t_vec2i *p0, t_vec2i *p1, float v)
{
	int dx = p1->x - p0->x;
	int dy = p1->y - p0->y;
	if (abs(dx) > abs(dy)) {
		float a = (float) dy / (float) dx;
		float b = - a * (float) p0->x + (float) p0->y;
		for (int k = min(p0->x, p1->x); k <= max(p0->x, p1->x); k++)
			CR_draw(w, &(t_vec2i) {k, (int) round(a * (float) k + b)}, v);
	} else {
		float a = (float) dx / (float) dy;
		float b = - a * (float) p0->y + (float) p0->x;
		for (int k = min(p0->y, p1->y); k <= max(p0->y, p1->y); k++)
			CR_draw(w, &(t_vec2i) {(int) round(a * (float) k + b), k}, v);
	}
}

void	CR_draw_linef(t_window *w, t_vec2 *p0, t_vec2 *p1, float v)
{
	t_vec2i pi0, pi1;
	unnormalize(w, p0, &pi0);
	unnormalize(w, p1, &pi1);
	CR_draw_line(w, &pi0, &pi1, v);
}

void	CR_draw_triangle(t_window *w, t_vec2i *p0, t_vec2i *p1, t_vec2i *p2, float v)
{
	CR_draw_line(w, p0, p1, v);
	CR_draw_line(w, p1, p2, v);
	CR_draw_line(w, p2, p0, v);
}

void	CR_draw_trianglef(t_window *w, t_vec2 *p0, t_vec2 *p1, t_vec2 *p2, float v)
{
	t_vec2i pi0, pi1, pi2;
	unnormalize(w, p0, &pi0);
	unnormalize(w, p1, &pi1);
	unnormalize(w, p2, &pi2);
	CR_draw_triangle(w, &pi0, &pi1, &pi2, v);
}

float det2(t_vec2i *a, t_vec2i *b)
{
	return (a->x * b->y - a->y * b->x);
}

void	CR_fill_triangle(t_window *w, t_vec2i *p0, t_vec2i *p1, t_vec2i *p2, float va)
{
	t_vec2i min_vec = (t_vec2i) {min(p0->x, min(p1->x, p2->x)), min(p0->y, min(p1->y, p2->y))};
	t_vec2i max_vec = (t_vec2i) {max(p0->x, max(p1->x, p2->x)), max(p0->y, max(p1->y, p2->y))};
	t_vec2i vp;
	for (vp.x = min_vec.x; vp.x <= max_vec.x; vp.x++)
	{
		for (vp.y = min_vec.y; vp.y <= max_vec.y; vp.y++)
		{
			t_vec2i v = {vp.x, vp.y};
			t_vec2i v0 = { p0->x, p0->y };
			t_vec2i v1 = { p1->x - p0->x, p1->y - p0->y };
			t_vec2i v2 = { p2->x - p0->x, p2->y - p0->y };
			float a = (det2(&v, &v2) - det2(&v0, &v2)) / det2(&v1, &v2);
			float b = -(det2(&v, &v1) - det2(&v0, &v1)) / det2(&v1, &v2);
			if (a + b <= 1.0f && a >= 0.0f && b >= 0.0f)
				CR_draw(w, &vp, va);
		}
	}
}	

void	CR_fill_trianglef(t_window *w, t_vec2 *p0, t_vec2 *p1, t_vec2 *p2, float va)
{
	t_vec2i pi0, pi1, pi2;
	unnormalize(w, p0, &pi0);
	unnormalize(w, p1, &pi1);
	unnormalize(w, p2, &pi2);
	CR_fill_triangle(w, &pi0, &pi1, &pi2, va);
}