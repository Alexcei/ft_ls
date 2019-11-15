#include "ft_ls.h"

void    ft_fill_tree_data(t_avltree1 **root, unsigned flag, t_data *data_subtree)
{
    if (flag & FLAG_T)
        ft_avlt_insert1(root, data_subtree, ft_date_cmp);
    else
        ft_avlt_insert1(root, data_subtree, ft_name_cmp);
    free(data_subtree);
}

static void	    ft_get_data2(char *d_name, t_data *data, t_avltree1 **root, t_width *width)
{
    char		*tmp;
    char        *res;
    t_data		*data_subtree;

    tmp = ft_strjoin(data->dir, "/");
    res = ft_strjoin(tmp, d_name);
    free(tmp);
    if (!is_hidden(d_name, data->flag) || data->flag & FLAG_A)
    {
        data_subtree = ft_get_data(res, data->flag, data->level + 1, 0, width);
        if (data_subtree)
        {
            data->nb_files = data->nb_files + 1;
            data->total_size += (data_subtree->stats).st_blocks;
            ft_fill_tree_data(root, data->flag, data_subtree);
        }
    }
    ft_strdel(&res);
}

void    ft_set_width(t_data *data, t_width *width)
{
    data->width->w_st_nlink = width->w_st_nlink;
    data->width->w_gr_name = width->w_gr_name;
    data->width->w_pw_name = width->w_pw_name;
    data->width->w_st_size = width->w_st_size;
}


void    btree_apply_infix_w(t_avltree1 *root, void (*applyf)(t_data*, t_width*), t_width *width)
{
    if (root == 0)
        return ;
    btree_apply_infix_w(root->left, (*applyf), width);
    (*applyf)(root->data, width);
    btree_apply_infix_w(root->right, (*applyf), width);
}

t_avltree1   *ft_creat_subtree(t_data *data)
{
    t_avltree1       *subtree;
    struct dirent	 *dirent;
    DIR				*d;
    t_width     width;

    errno = 0;
    subtree = NULL;
    ft_bzero(&width, sizeof(t_width));
    if (!(d = opendir(data->dir)))
        return (NULL);
    while ((dirent = readdir(d)))
        ft_get_data2(dirent->d_name, data, &subtree, &width);
    closedir(d);
    btree_apply_infix_w(subtree, ft_set_width, &width);
    return (subtree);
}