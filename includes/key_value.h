#include <stdlib.h>

typedef struct s_key_value	t_key_value;

void		add_kv_back(t_key_value **list, t_key_value *new);
void		delete_kv_node(t_key_value **list, char *key);
void		free_kv_node(t_key_value *node);
void		free_kv_list(t_key_value *list);
char		*get_kv_value(t_key_value *list, char *key);
char		*get_kv_n_value(t_key_value *list, char *key, size_t n);
char		*get_key(t_key_value *node);
char		*get_value(t_key_value *node);
t_key_value	*get_next(t_key_value *node);
t_key_value	*kv_chr(t_key_value *list, char *key);
t_key_value	*kv_n_chr(t_key_value *list, char *key, size_t n);
size_t		kv_list_len(t_key_value *list);
void		modify_kv_list(t_key_value **kv, char *new, ssize_t key_len);
t_key_value	*new_kv_node(char *new_kv);
int			set_kv_value(t_key_value *node, char *new, size_t name_len);
