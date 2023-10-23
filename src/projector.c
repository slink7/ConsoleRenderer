#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "t_linked_list.h"
#include "file.h"
#include "projector.h"
#include "t_window.h"

void free_mesh(t_mesh *obj)
{
	free(obj->vertices);
	free(obj->triangles);
}

void free_object(t_object *obj)
{
	free(obj->mesh.triangles);
	free(obj->mesh.vertices);
	free(obj);
}

t_object	*read_obj(char *path)
{
	char *file = get_file(path);
	int index = 0;
	t_link *vertices = 0;
	t_link *triangles = 0;
	while (file[index])
	{
		if (file[index] == 'v') {
			t_vec3 *v = malloc(sizeof(t_vec3));
			int vi = 0;
			while (file[++index] != '\n')
				if (vi < 3 && (file[index] >= '0' && file[index] <= '9' || file[index] == '-') && file[index - 1] == ' ')
					switch (vi++) {
						case 0 : v->x = (float) atof(&file[index]); break;
						case 1 : v->y = (float) atof(&file[index]); break;
						case 2 : v->z = (float) atof(&file[index]); break;
					}
			push_back_link(&vertices, (void *)v);
		}
		if (file[index] == 'f')
			while (file[++index] != '\n')
				if (file[index] >= '0' && file[index] <= '9' && file[index - 1] == ' ')
				{
					int *v = malloc(sizeof(int));
					*v = atoi(&file[index]);
					push_back_link(&triangles, (void *)v);
				}
		index++;
	}
	free(file);
	t_object *out = malloc(sizeof(t_object));

	out->mesh.vertices_count = get_length_link(vertices);
	t_vec3 **tempv = (t_vec3**)to_array_link(vertices);
	free_link(&vertices);
	out->mesh.vertices = malloc(sizeof(t_vec3) * out->mesh.vertices_count);
	for (int k = 0; k < out->mesh.vertices_count; k++) {
		out->mesh.vertices[k] = (t_vec3) { tempv[k]->x, tempv[k]->y, tempv[k]->z };
		free(tempv[k]);
	}
	free(tempv);

	out->mesh.triangle_count = get_length_link(triangles) / 3;
	int **tempt = (int**)to_array_link(triangles);
	free_link(&triangles);
	out->mesh.triangles = malloc(sizeof(int) * out->mesh.triangle_count * 3);
	for (int k = 0; k < out->mesh.triangle_count * 3; k++) {
		out->mesh.triangles[k] = *(tempt[k]) - 1;
		free(tempt[k]);
	}
	free(tempt);

	out->pos = (t_vec3){ 0.0f, 0.0f, 0.0f };
	out->rot = (t_vec3){ 0.0f, 0.0f, 0.0f };
	return out;
}

void	print_matrix(t_matrix *m)
{
	printf("{\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n\t%f, %f, %f, %f\n}\n", (*m)[0][0], (*m)[1][0], (*m)[2][0], (*m)[3][0], (*m)[0][1], (*m)[1][1], (*m)[2][1], (*m)[3][1], (*m)[0][2], (*m)[1][2], (*m)[2][2], (*m)[3][2], (*m)[0][3], (*m)[1][3], (*m)[2][3], (*m)[3][3]);
}

void	print_vec(t_vec3 *v)
{
	printf("(%f, %f, %f)\n", v->x, v->y, v->z);
}

void	fill_matrix(t_matrix *out, float value)
{
	for(int k = 0; k < 4; k++)
		for(int l = 0; l < 4; l++)
			(*out)[k][l] = value;
}

void	set_rot_matrix_X(t_matrix *out, float theta)
{
	fill_matrix(out, 0.0f);
	(*out)[0][0] = 1.0f;
	(*out)[1][1] = cosf(theta);
	(*out)[1][2] = sinf(theta);
	(*out)[2][1] = -sinf(theta);
	(*out)[2][2] = cosf(theta);
}

void	set_rot_matrix_Y(t_matrix *out, float theta)
{
	fill_matrix(out, 0.0f);
	(*out)[0][0] = cosf(theta);
	(*out)[2][0] = sinf(theta);
	(*out)[1][1] = 1.0f;
	(*out)[0][2] = -sinf(theta);
	(*out)[2][2] = cosf(theta);
}

void	set_rot_matrix_Z(t_matrix *out, float theta)
{
	fill_matrix(out, 0.0f);
	(*out)[0][0] = cosf(theta);
	(*out)[1][0] = -sinf(theta);
	(*out)[0][1] = sinf(theta);
	(*out)[1][1] = cosf(theta);
	(*out)[2][2] = 1.0f;
}

void	matrix_proj(t_vec3 *a, t_matrix *m, t_vec3 *out)
{
	out->x = (a->x * (*m)[0][0] + a->y * (*m)[1][0] + a->z * (*m)[2][0] + (*m)[3][0]);
	out->y = (a->x * (*m)[0][1] + a->y * (*m)[1][1] + a->z * (*m)[2][1] + (*m)[3][1]);
	out->z = (a->x * (*m)[0][2] + a->y * (*m)[1][2] + a->z * (*m)[2][2] + (*m)[3][2]);
	float w = (a->x * (*m)[0][3] + a->y * (*m)[1][3] + a->z * (*m)[2][3] + (*m)[3][3]);
	
	if (w != 0.0f) {
		out->x /= w;
		out->y /= w;
		out->z /= w;
	}
}

void	cross_product(t_vec3 *A, t_vec3 *B, t_vec3 *out)
{
	out->x = A->y * B->z - A->z * B->y;
	out->y = A->z * B->x - A->x * B->z;
	out->z = A->x * B->y - A->y * B->x;
}

float		dot_product(t_vec3 *A, t_vec3 *B)
{
	return A->x * B->x + A->y * B->y + A->z * B->z;
}

void	vec_sub(t_vec3 *A, t_vec3 *B, t_vec3 *out)
{
	out->x = A->x - B->x;
	out->y = A->y - B->y;
	out->z = A->z - B->z;
}

float	vec_length(t_vec3 *A)
{
	return sqrtf(A->x * A->x + A->y * A->y + A->z * A->z);
}

void	CR_proj(t_window *w, t_object *o, int draw_edges)
{
	float ncp = 0.1f;
	float fcp = 10000.0f;
	float fov = 70.0f;
	float ar = (float) w->height / (float) w->width;
	float fov_rad = 1.0f / tanf(fov * 0.5f );

	t_matrix proj = {{0.0f}};

	t_vec3 light_dir = { -sqrt(2.0f) / 2.0f, 0.0f, -sqrt(2.0f) / 2.0f };

	proj[0][0] = ar * fov_rad;
	proj[1][1] = fov_rad;
	proj[2][2] = fcp / (fcp - ncp);
	proj[3][2] = (-fcp * ncp) / (fcp - ncp);
	proj[2][3] = 1.0f;
	proj[3][3] = 0.0f;

	int drawn = 0;

	for (int k = 0; k < o->mesh.triangle_count; k++)
	{
		t_vec3 trio[3] = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
		t_vec3 triZ[3] = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
		t_vec3 triZY[3] = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
		t_vec3 triZYX[3] = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
		t_vec3 triZYXT[3] = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
		for (int l = 0; l < 3; l++) {
			t_matrix m = {
				{0.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 0.0f}
			};
			set_rot_matrix_Z(&m, o->rot.z);
			matrix_proj(&o->mesh.vertices[o->mesh.triangles[3 * k + l]], &m, &triZ[l]);
			set_rot_matrix_Y(&m, o->rot.y);
			matrix_proj(&triZ[l], &m, &triZY[l]);
			set_rot_matrix_X(&m, o->rot.x);
			matrix_proj(&triZY[l], &m, &triZYX[l]);
			triZYXT[l].x = triZYX[l].x + o->pos.x;
			triZYXT[l].y = triZYX[l].y + o->pos.y;
			triZYXT[l].z = triZYX[l].z + o->pos.z;
			matrix_proj(&triZYXT[l], &proj, &trio[l]);
		}
		t_vec3 v0;
		vec_sub(&triZYXT[1], &triZYXT[0], &v0);
		t_vec3 v1;
		vec_sub(&triZYXT[2], &triZYXT[0], &v1);
		t_vec3 normal;
		cross_product(&v0, &v1, &normal);
		float l = vec_length(&normal);
		normal.x /= l;
		normal.y /= l;
		normal.z /= l;
		t_vec3 to_cam;
		vec_sub(&triZYXT[0], &(t_vec3) {0, 0, 0}, &to_cam);

		if (dot_product(&to_cam, &normal) <= 0.0f)
		{
			float light = fmin(1.0f, fmax(0.05f, dot_product(&light_dir, &normal)));
			drawn++;
			CR_fill_trianglef(w, &(t_vec2){ trio[0].x, trio[0].y },
								 &(t_vec2){ trio[1].x, trio[1].y },
								 &(t_vec2){ trio[2].x, trio[2].y }, light);
			if (draw_edges)
				CR_draw_trianglef(w, &(t_vec2){ trio[0].x, trio[0].y },
									&(t_vec2){ trio[1].x, trio[1].y },
									&(t_vec2){ trio[2].x, trio[2].y }, 1.0f);
		}
	}
}