#include "ft_ls.h"

int         ft_name_cmp(t_data *data1, t_data *data2)
{
    if (data1->flag & FLAG_R)
        return (ft_strcmp(data2->dir, data1->dir));
    else
        return (ft_strcmp(data1->dir, data2->dir));
}

int         ft_date_cmp(t_data *data1, t_data *data2)
{
    long long ret;

    if (data1->flag & FLAG_R)
        ret = data1->sec - data2->sec;
    else
        ret = data2->sec - data1->sec;
    if (ret != 0)
        return (ret > 0 ? 1 : -1);
    return (ft_name_cmp(data1, data2));
}


void    btree_apply_infix(t_avltree1 *root, void (*applyf)(t_data*, unsigned *i), unsigned *flag)
{
    if (root == 0)
        return ;
    btree_apply_infix(root->left, (*applyf), flag);
    (*applyf)(root->data, flag);
    btree_apply_infix(root->right, (*applyf), flag);
}

int	        is_hidden(char *name)
{
    return (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0 || name[0] == '.');
}