#ifndef T_LINKED_LIST
#define T_LINKED_LIST

typedef struct s_link t_link;

typedef struct s_link
{
	t_link *next;
	void *data;
}	t_link;

t_link	*create_link(void *data);
void	free_link(t_link **link);
void	print_link(t_link *link);
void	push_back_link(t_link **link, void *data);
void	push_front_link(t_link **link, void *data);
void	*get_at_link(t_link *link, int i);
void	**to_array_link(t_link *link);
int		get_length_link(t_link *link);

#endif