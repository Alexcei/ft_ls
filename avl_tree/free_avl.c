#include "avltree.h"

static void     free_node(t_avltree *node)
{
    if (node)
    {
        ft_memdel((void *)(&(node->content)));
        node->content_size = 0;
        ft_memdel((void *)(&node));
    }
}

static void     btree_apply_suffix(t_avltree *root, void (*f)(t_avltree *))
{
    if (root == NULL)
        return ;
    btree_apply_suffix(root->left, f);
    btree_apply_suffix(root->right, f);
    f(root);
}

void		ft_avlt_free(t_avltree **root)
{
    if (root == NULL || *root == NULL)
        return ;
    btree_apply_suffix(*root, &free_node);
    *root = NULL;
}