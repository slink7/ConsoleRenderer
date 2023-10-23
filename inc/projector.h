#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "t_window.h"

typedef struct s_vec3
{
	float x;
	float y;
	float z;
}	t_vec3;

typedef struct s_mesh
{
	t_vec3 *vertices;
	int vertices_count;
	int *triangles;
	int triangle_count;
}	t_mesh;

typedef struct s_object
{
	t_mesh mesh;
	t_vec3 pos;
	t_vec3 rot;
}	t_object;

typedef float t_matrix[4][4];

void 	free_mesh(t_mesh *obj)
void 	free_object(t_object *obj)
void	CR_proj(t_window *w, t_object *o, int draw_edges);

#endif