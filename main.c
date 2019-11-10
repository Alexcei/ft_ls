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

int	        is_hidden(char *name)
{
    return (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0 || name[0] == '.');
}

t_data			*ft_get_data(char *dir, unsigned flag, int level, int count);

static void	    fill_with_file(char *d_name, t_data *data, t_avltree1 **root)
{
    char		*buff;
    t_data		*data_subtree;

    if (!(buff = (char *)ft_memalloc(sizeof(char) * (ft_strlen(data->dir) + 5 + ft_strlen(d_name)))))
        return ;
    ft_strcat(buff, data->dir);
    ft_strcat(buff, "/");
    ft_strcat(buff, d_name);
    if (!is_hidden(d_name) || data->flag & FLAG_A)
    {
        data_subtree = ft_get_data(buff, data->flag, data->level + 1, 0);
        if (data_subtree)
        {
            if (data->flag & FLAG_T)
                ft_avlt_insert1(root, data_subtree, ft_date_cmp);
            else
                ft_avlt_insert1(root, data_subtree, ft_name_cmp);
            data->nb_files = data->nb_files + 1;
            data->total_size = data->total_size + (data_subtree->stats).st_blocks;
        }
    }
    ft_memdel((void **)&buff);
}

t_avltree1   *ft_creat_subtree(t_data *data) //, unsigned flag, int level, int count)
{
    t_avltree1       *subtree;
    struct dirent	 *dirent;
    DIR				*d;

    subtree = NULL;
    if (!(d = opendir(data->dir)))
    {
        data->st_mode = 0;
        return (NULL);
    }
    while ((dirent = readdir(d)))
    {
        if (!is_hidden(dirent->d_name) || !data->level)
        fill_with_file(dirent->d_name, data, &subtree);
    }
    closedir(d);
    return (subtree);
}

t_data	*ft_get_values(t_data *data, unsigned flag, int level, int count)
{
    struct passwd	*pwd;
    struct group	*grp;

    data->level = level;
    data->count = count;
    data->flag = flag;
    data->sec = (long)(data->stats).st_birthtimespec.tv_sec;
    if (!is_hidden(data->filename) || (!data->level))     //  && !data->count
        data->subtree = ft_creat_subtree(data);
    else
        data->subtree = NULL;
    data->total_size = 0;
    if ((pwd = getpwuid((data->stats).st_uid)))
        data->pw_name = pwd->pw_name;
    else
        data->pw_name = NULL;
    if ((grp = getgrgid((data->stats).st_gid)))
        data->gr_name = grp->gr_name;
    else
        data->gr_name = NULL;
    return (data);
}

char	*ft_get_filename(t_data *data)
{
    char	*file_name;
    char	*buff;

    file_name = ft_strrchr(data->dir, '/');
    if (!(buff = (char *)ft_memalloc(sizeof(char) * (ft_strlen(data->dir) + 40))))
        return (NULL);
    if (ft_strcmp("/", data->dir) == 0 || file_name == NULL)
        ft_strcat(buff, data->dir);
    else if (ft_strlen(file_name) > 1)
        ft_strcat(buff, file_name + 1);
    else
    {
        ft_strcat(buff, data->dir);
        buff[ft_strlen(file_name) - 1] = '\0';
    }
    return (buff);
}

t_data			*ft_get_data(char *dir, unsigned flag, int level, int count)
{
    t_data	    *data;
    struct stat stats;

    errno = 0;
    if (lstat(dir, &stats) == -1)
        ft_fprintf(1,"ls: %s: %s\n", dir, strerror(errno));
    if (!(data = (t_data*)ft_memalloc(sizeof(t_data))))
        return (NULL);
    data->stats = stats;
    data->st_mode = stats.st_mode;
    data->dir = ft_strdup(dir);
    data->filename = ft_get_filename(data);
    if (!is_hidden(data->filename) || (!data->count && !data->level))
        data = ft_get_values(data, flag, level, count);
    return (data);
}

int     ft_creat_data(t_avltree1	**root, int ac, char **av, unsigned flag)
{
    t_data	*data;
    int     i;
    int     args;

    i = 0;
    args = ac - 1;
    if (ac == 1)
    {

        data = ft_get_data(".", flag, 0, i);
        if (!data)
            return (1);
        data->args = args;
        ft_avlt_insert1(root, data, ft_name_cmp);
    }
    while (i < ac - 1)
    {
        data = ft_get_data(av[i], flag, 0, i);
        if (!data)
            return (1);
        data->args = args;
        ft_avlt_insert1(root, data, ft_name_cmp);
        i++;
    }
    return (0);
}

void    print_files(t_data *data, unsigned flag)
{
    int i;

    if (flag == 0)
        i = 2;
    ft_printf("%s\n", data->filename);
}

void    btree_apply_infix(t_avltree1 *root, void (*applyf)(t_data*, unsigned i), unsigned flag)
{
    if (root == 0)
        return ;
    btree_apply_infix(root->left, (*applyf), flag);
    (*applyf)(root->data, flag);
    btree_apply_infix(root->right, (*applyf), flag);
}

void    recursive(t_data *data, unsigned flag)
{
    if (S_ISDIR(data->st_mode))
    {
        if (data->count || data->level)
            ft_printf("\n");
        if ((data->args > 1 || data->level))
            ft_printf("%s:\n", data->dir);
    }
    btree_apply_infix(data->subtree, print_files, flag);
    if (flag & FLAG_R_UP)
        btree_apply_infix(data->subtree, recursive, flag);
}

int			main(int ac, char **av)
{
    t_avltree1	*root_content;
    unsigned    flag;

    av++;
    flag = 0;
    if (ft_ls_parse(&ac, &av, &flag))
        return (1);
    if (ft_creat_data(&root_content, ac, av, flag) == 1)
    {
        ft_avlt_free1(&root_content);
        return (1);
    }
    btree_apply_infix(root_content, recursive, flag);
    ft_avlt_free1(&root_content);
    return (0);
}