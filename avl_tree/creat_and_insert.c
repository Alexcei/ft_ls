#include "avltree.h"

static t_avltree	*ft_avlt_create(void *content, size_t content_size)
{
    t_avltree	*node;
    void		*content_cpy;

    node = (t_avltree *)ft_memalloc(sizeof(t_avltree));
    if (node == NULL)
        return (NULL);
    content_cpy = (void *)ft_memalloc(content_size);
    if (content_cpy == NULL)
    {
        ft_memdel((void *)&node);
        return (NULL);
    }
    ft_memcpy(content_cpy, content, content_size);
    node->content = content_cpy;
    node->content_size = content_size;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

void    ft_avlt_insert(t_avltree **root, void *content, size_t content_size, int (*cmp)(void *,void *))
{
    if (!*root || !(*root)->content)
    {
        *root = ft_avlt_create(content, content_size);
    }
    else
    {
        if (cmp((*root)->content, content) == 0)
            ft_avlt_insert(&((*root)->left), content, content_size, cmp);
        else
            ft_avlt_insert(&((*root)->right), content, content_size, cmp);
        *root = balance(*root);
    }
}