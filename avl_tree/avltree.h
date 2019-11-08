#ifndef AVLTREE_H
# define AVLTREE_H

#include "../ft_printf/libft/libft.h"

typedef struct            s_avltree
{
    void				  *content;
    size_t				  content_size;
    unsigned char         height;
    struct s_avltree      *left;
    struct s_avltree      *right;
}                         t_avltree;

void		    ft_avlt_free(t_avltree **root);
void            ft_avlt_insert(t_avltree **root, void *content, size_t content_size, int (*cmp)(void *,void *));
unsigned char   height(t_avltree* p);
t_avltree       *balance(t_avltree* p);

#endif