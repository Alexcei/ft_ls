#include "ft_ls.h"

t_data			*ft_get_data(char *dir, unsigned flag, int level, int count, t_width *width);

static void	    fill_with_file(char *d_name, t_data *data, t_avltree1 **root, t_width *width)
{
    char		*buff;
    t_data		*data_subtree;

    if (!(buff = (char *)ft_memalloc(sizeof(char) * (ft_strlen(data->dir) + 5 + ft_strlen(d_name)))))
        return ;
    ft_strcat(buff, data->dir);
    ft_strcat(buff, "/");
    ft_strcat(buff, d_name);
    if (!is_hidden(d_name, data->flag) || data->flag & FLAG_A)
    {
        data_subtree = ft_get_data(buff, data->flag, data->level + 1, 0, width);
        if (data_subtree)
        {
            if (data->flag & FLAG_T)
                ft_avlt_insert1(root, data_subtree, ft_date_cmp);
            else
                ft_avlt_insert1(root, data_subtree, ft_name_cmp);
            data->nb_files = data->nb_files + 1;
            data->total_size += (data_subtree->stats).st_blocks;
            free(data_subtree);
        }
    }
    ft_memdel((void **)&buff);
}

void    btree_apply_infix_w(t_avltree1 *root, void (*applyf)(t_data*, t_width*), t_width *width)
{
    if (root == 0)
        return ;
    btree_apply_infix_w(root->left, (*applyf), width);
    (*applyf)(root->data, width);
    btree_apply_infix_w(root->right, (*applyf), width);
}

void    ft_set_width(t_data *data, t_width *width)
{
    data->width->w_st_nlink = width->w_st_nlink;
    data->width->w_gr_name = width->w_gr_name;
    data->width->w_pw_name = width->w_pw_name;
    data->width->w_st_size = width->w_st_size;
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
    {
        //data->st_mode = 0;
        return (NULL);
    }
    while ((dirent = readdir(d)))
        fill_with_file(dirent->d_name, data, &subtree, &width);
    closedir(d);
    btree_apply_infix_w(subtree, ft_set_width, &width);
    return (subtree);
}

t_data	*ft_get_values(t_data *data, unsigned flag, int level, int count, t_width *width)
{
    struct passwd	*pwd;
    struct group	*grp;

    data->level = level;
    data->count = count;
    data->flag = flag;
    data->time = (data->stats).st_mtime;
    if (!is_hidden(data->filename, data->flag) || (!data->level))
        data->subtree = ft_creat_subtree(data);
    else
        data->subtree = NULL;
    if ((pwd = getpwuid((data->stats).st_uid)))
    {
        data->pw_name = pwd->pw_name;
        width->w_pw_name = MAX((int)ft_strlen(data->pw_name), width->w_pw_name); // + 1!!
    }
    else
        data->pw_name = NULL;
    if ((grp = getgrgid((data->stats).st_gid)))
    {
        data->gr_name = grp->gr_name;
        width->w_gr_name = MAX((int)ft_strlen(data->gr_name), width->w_gr_name);
    }
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

t_data			*ft_get_data(char *dir, unsigned flag, int level, int count, t_width *width)
{
    t_data	    *data;
    struct stat stats;
    int num_len;
    DIR				*d;

    errno = 0;
    if (lstat(dir, &stats) == -1)
        ft_fprintf(2,"ls: %s: %s\n", dir, strerror(errno));
    if (!(data = (t_data*)ft_memalloc(sizeof(t_data))))
        return (NULL);
    data->width = ft_memalloc(sizeof(t_width));
    data->stats = stats;
    data->st_mode = stats.st_mode;
    num_len = ft_fprintf(-1, "%lld", data->stats.st_nlink) + 1;
    width->w_st_nlink = MAX(num_len, width->w_st_nlink);
    data->dir = ft_strdup(dir);
    errno = 0;
    if (!(d = opendir(data->dir)) && S_ISDIR(data->st_mode) && (!level || flag & FLAG_R_UP))          // !!!!!!!!!!!!!!!!!!
        ft_fprintf(2,"ls: %s: %s\n", data->dir, strerror(errno));  // !!!!!!!!!!!!
    num_len = ft_fprintf(-1, "%lld", stats.st_size);
    width->w_st_size = MAX(num_len, width->w_st_size);
    data->filename = ft_get_filename(data);
    if (!is_hidden(data->filename, data->flag) || (!data->count && !data->level))
        data = ft_get_values(data, flag, level, count, width);
    return (data);
}

int     ft_creat_data(t_avltree1	**root, int ac, char **av, unsigned flag)
{
    t_data	*data;
    int     i;
    int     args;
    t_width width;

    i = 0;
    ft_bzero(&width, sizeof(t_width));
    args = ac - 1;
    if (ac == 1)
    {
        data = ft_get_data(".", flag, 0, i, &width);
        if (!data)
            return (1);
        data->args = args;
        if (data->flag & FLAG_T)
            ft_avlt_insert1(root, data, ft_date_cmp);
        else
            ft_avlt_insert1(root, data, ft_name_cmp);
        btree_apply_infix_w(*root, ft_set_width, &width);
        free(data);
    }
    while (i < ac - 1)
    {
        if (!av[i][0])
        {
            ft_fprintf(2, "ls: fts_open: No such file or directory\n");
            exit(1);
        }
        data = ft_get_data(av[i], flag, 0, i, &width);
        if (!data)
            return (1);
        data->args = args;
        if (data->flag & FLAG_T)
            ft_avlt_insert1(root, data, ft_date_cmp);
        else
            ft_avlt_insert1(root, data, ft_name_cmp);
        btree_apply_infix_w(*root, ft_set_width, &width);
        free(data);
        i++;
    }
    return (0);
}

void    recursive(t_data *data, unsigned *flag)
{
    if (S_ISLNK((data->stats).st_mode) && *flag &FLAG_R_UP)
        return;
    if (S_ISDIR((data->stats).st_mode) && (!is_hidden(data->filename, data->flag) || (data->args > 0 && !data->level)))   // || S_ISLNK((data->stats).st_mode)
    {
        if (*flag & FLAG_SPC)
            ft_printf("\n");
        else
            *flag |= 1u << 10u;
        if ((data->args > 1 || data->level))
            ft_printf("%s:\n", data->dir);
    }
    if (*flag & FLAG_L && data->nb_files)
    {
        ft_printf("total %d\n", data->total_size);
        *flag |= 1u << 10u;
    }
    btree_apply_infix(data->subtree, print_files, flag);
    if (data->st_mode)
        *flag |= 1u << 10u;
    if (*flag & FLAG_R_UP)
        btree_apply_infix(data->subtree, recursive, flag);
}

void    first_put(t_data *data, unsigned *flag)
{
    if (lstat(data->dir, &(data->stats)) == -1)
        return ;
    if (S_ISLNK((data->stats).st_mode) && *flag & FLAG_L)
        print_files(data, flag);
    else if (!S_ISDIR(data->st_mode) && *flag &FLAG_L)
        print_files(data, flag);
    else if (!S_ISDIR(data->st_mode))
    {
        if (!(S_ISLNK((data->stats).st_mode) && !data->level && data->args == 1 && data->subtree))
        {
            ft_printf("%s\n", data->filename);
            *flag |= 1u << 10u;
        }
    }
}

int			main(int ac, char **av)
{
    t_avltree1	*root_content;
    unsigned    flag;

    av++;
    flag = 0;
    root_content = NULL;
    if (ft_ls_parse(&ac, &av, &flag))
        return (1);
    if (ft_creat_data(&root_content, ac, av, flag) == 1)
    {
        ft_avlt_free1(&root_content);
        return (1);
    }
    btree_apply_infix(root_content, first_put, &flag);
    btree_apply_infix(root_content, recursive, &flag);
    ft_avlt_free1(&root_content);
    return (0);
}