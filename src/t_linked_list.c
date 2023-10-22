#include <stdio.h>
#include <stdlib.h>

#include "t_linked_list.h"

t_link	*create_link(void *data)
{
	t_link *out = malloc(sizeof(t_link));
	out->data = data;
	out->next = 0;
	return out;
}

void	free_link(t_link **link)
{
	if (!(*link))
		return ;
	free_link(&(*link)->next);
	free(*link);
}

void	print_link(t_link *link)
{
	if (!link) return;
	printf("[%ld]", (long) link->data);
	if (link->next) {
		printf("->");
		print_link(link->next);
	}
	else
		printf("\n");
}

void	push_back_link(t_link **link, void *data)
{
	if (!(*link)) {
		*link = create_link(data);
		return;
	}
	push_back_link(&(*link)->next, data);
}

void	push_front_link(t_link **link, void *data)
{
	if (!(*link)) {
		*link = create_link(data);
		return;
	}
	t_link *temp = create_link(data);
	temp->next = *link;
	*link = temp;
}

void	*get_at_link(t_link *link, int i)
{
	if (i < 1)
		return link->data;
	return get_at_link(link->next, i - 1);
}

void	**to_array_link(t_link *link)
{
	int len = get_length_link(link);
	void **out = malloc(sizeof(void *) * len);
	for (int k = 0; link; link = link->next) {
		out[k] = link->data;
		k++;
	}
	return out;
}

int		get_length_link(t_link *link)
{
	return (link->next) ? get_length_link(link->next) + 1 : 1;
}