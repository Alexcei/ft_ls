#include "ft_ls.h"

static int         bfactor(t_avltree1* p)
{
    return (height1(p->right) - height1(p->left));
}

static void            fixheight(t_avltree1* p)
{
    unsigned char hl;
    unsigned char hr;

    hl = height1(p->left);
    hr = height1(p->right);
    p->height1 = (hl > hr ? hl : hr) + 1;
}

static t_avltree1* rotateright(t_avltree1* r)
{
    t_avltree1* l;

    l = r->left;
    r->left = l->right;
    l->right = r;
    fixheight(r);
    fixheight(l);
    return (l);
}

static t_avltree1* rotateleft(t_avltree1* l)
{
    t_avltree1* r;

    r = l->right;
    l->right = r->left;
    r->left = l;
    fixheight(l);
    fixheight(r);
    return (r);
}

t_avltree1* balance1(t_avltree1* p)
{
    fixheight(p);
    if ((bfactor(p) == 2))
    {
        if (bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
            p->left = rotateleft(p->left);
        return (rotateright(p));
    }
    return (p);
}

unsigned char   height1(t_avltree1* p)
{
    return (p ? p->height1 : 0);
}

static t_avltree1	*ft_avlt_create(t_data *data)
{
    t_avltree1	*node;
    t_data		*data_cpy;

    node = (t_avltree1 *)ft_memalloc(sizeof(t_avltree1));
    if (node == NULL)
        return (NULL);
    data_cpy = (t_data *)ft_memalloc(sizeof(t_data));
    if (data_cpy == NULL)
    {
        ft_memdel((void *)&node);
        return (NULL);
    }
    ft_memcpy(data_cpy, data, sizeof(t_data));
    node->data = data_cpy;
    node->height1 = 1;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

void    ft_avlt_insert1(t_avltree1 **root, t_data *data, int (*cmp)(t_data *,t_data *))
{
    if (!*root || !(*root)->data)
    {
        *root = ft_avlt_create(data);
    }
    else
    {
        if (cmp((*root)->data, data) > 0)
            ft_avlt_insert1(&((*root)->left), data, cmp);
        else
            ft_avlt_insert1(&((*root)->right), data, cmp);
        *root = balance1(*root);
    }
}

static void     free_node(t_avltree1 *node)
{
    if (node)
    {
        ft_avlt_free1(&(node->data->subtree));
        ft_memdel((void *)(&(node->data->width)));
        ft_memdel((void *)(&(node->data->filename)));
        ft_memdel((void *)(&(node->data->dir)));
        ft_memdel((void *)(&(node->data)));
        ft_memdel((void *)&node);
    }
}

static void     btree_apply_suffix(t_avltree1 *root, void (*f)(t_avltree1 *))
{
    if (root == NULL)
        return ;
    btree_apply_suffix(root->left, f);
    btree_apply_suffix(root->right, f);
    f(root);
}

void		ft_avlt_free1(t_avltree1 **root)
{
    if (root == NULL || *root == NULL)
        return ;
    btree_apply_suffix(*root, &free_node);
    *root = NULL;
}