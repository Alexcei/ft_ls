#include "ft_ls.h"

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
            free(data_subtree);
        }
    }
    ft_memdel((void **)&buff);
}

t_avltree1   *ft_creat_subtree(t_data *data)
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
        fill_with_file(dirent->d_name, data, &subtree);
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
    data->time = (data->stats).st_mtime;
    data->sec = (long)(data->stats).st_birthtimespec.tv_sec;
    if (!is_hidden(data->filename) || (!data->level))
        data->subtree = ft_creat_subtree(data);
    else
        data->subtree = NULL;
    data->total_size = 0;
    if ((pwd = getpwuid((data->stats).st_uid)))
    {
        data->pw_name = pwd->pw_name;
        data->w_pw_name = MAX((int)ft_strlen(data->pw_name) + 1, data->w_pw_name);
    }
    else
        data->pw_name = NULL;
    if ((grp = getgrgid((data->stats).st_gid)))
    {
        data->gr_name = grp->gr_name;
        data->w_gr_name = MAX((int)ft_strlen(data->gr_name), data->w_gr_name);
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

t_data			*ft_get_data(char *dir, unsigned flag, int level, int count)
{
    t_data	    *data;
    struct stat stats;
    int num_len;

    errno = 0;
    if (lstat(dir, &stats) == -1)
        ft_fprintf(1,"ls: %s: %s\n", dir, strerror(errno));
    if (!(data = (t_data*)ft_memalloc(sizeof(t_data))))
        return (NULL);
    data->stats = stats;
    data->st_mode = stats.st_mode;
    num_len = ft_fprintf(-1, "%lld", data->stats.st_nlink);
    data->w_st_nlink = MAX(num_len, data->w_st_nlink);
    data->w_st_nlink++;
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
        free(data);
    }
    while (i < ac - 1)
    {
        data = ft_get_data(av[i], flag, 0, i);
        if (!data)
            return (1);
        data->args = args;
        ft_avlt_insert1(root, data, ft_name_cmp);
        free(data);
        i++;
    }
    return (0);
}

void	print_type(int mode)
{
    char		c;

    if (S_ISREG(mode))
        c = '-';
    else if (S_ISDIR(mode))
        c = 'd';
    else if (S_ISBLK(mode))
        c = 'b';
    else if (S_ISCHR(mode))
        c = 'c';
    else if (S_ISLNK(mode))
        c = 'l';
    else if (S_ISSOCK(mode))
        c = 's';
    else
        c = '-';
    ft_printf("%c", c);
}

void	print_permissions(t_data *data)
{
    int			mode;

    mode = data->stats.st_mode;
    print_type(mode);
    ft_printf((mode & S_IRUSR) ? "r" : "-");
    ft_printf((mode & S_IWUSR) ? "w" : "-");
    if (mode & S_ISUID)
        ft_printf((mode & S_IXUSR) ? "s" : "S");
    else
        ft_printf((mode & S_IXUSR) ? "x" : "-");
    ft_printf((mode & S_IRGRP) ? "r" : "-");
    ft_printf((mode & S_IWGRP) ? "w" : "-");
    if (mode & S_ISGID)
        ft_printf((mode & S_IXGRP) ? "s" : "S");
    else
        ft_printf((mode & S_IXGRP) ? "x" : "-");
    ft_printf((mode & S_IROTH) ? "r" : "-");
    ft_printf((mode & S_IWOTH) ? "w" : "-");
    if (mode & S_ISVTX)
        ft_printf((mode & S_IXUSR) ? "t" : "T");
    else
        ft_printf((mode & S_IXOTH) ? "x" : "-");
}
/*
void	print_file(t_data *data)
{
    struct stat		stats;
    struct passwd	*pwd;
    struct group	*grp;


    stats = data->stats;
    pwd = getpwuid(stats.st_uid);
    grp = getgrgid(stats.st_gid);
    print_permissions(file);
    ft_printf(" %*d %-*s %-*s ", widths[0], stats.st_nlink, widths[1], pwd->pw_name, widths[2], grp->gr_name);
    ft_printf(" %*lld %s ", widths[3], stats.st_size, time_string(stats.st_mtime, args));
    if (S_ISLNK(stats.st_mode) && args.l)
        print_link(file, args);
    else
        ft_printf("%s\n", file->filename);
}

void	set_widths(int widths[5], t_list *files)
{
    struct stat		stats;
    struct passwd	*pwd;
    struct group	*grp;
    int				num_len;

    widths[0] = 0;
    widths[1] = 0;
    widths[2] = 0;
    widths[3] = 0;
    widths[4] = 0;
    while (files)
    {
        stats = ((t_path *)files->content)->stats;
        pwd = getpwuid(stats.st_uid);
        grp = getgrgid(stats.st_gid);
        num_len = ft_fprintf(-1, "%lld", stats.st_nlink);
        widths[0] = MAX(num_len, widths[0]);
        widths[1] = MAX((int)ft_strlen(pwd->pw_name) + 1, widths[1]);
        widths[2] = MAX((int)ft_strlen(grp->gr_name), widths[2]);
        num_len = ft_dprintf(-1, "%lld", stats.st_size);
        widths[3] = MAX(num_len, widths[3]);
        widths[4] = MAX(year_length(stats.st_mtime), widths[4]);
        files = files->next;
    }
    widths[0]++;
}*/

static int	ft_nblen(char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i] != '\n')
        i++;
    return (i);
}

static char	*ft_strtim_return(char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i] == ' ')
        i++;
    return (str + i);
}

# define SIXMONTHS ((365 / 2) * 86400)

void		ft_print_date(t_data *data)
{
    char	*string;
    time_t	now;

    now = time(NULL);
    string = ctime((time_t *)&(data->time));
    ft_printf("%3.3s ", string + 4);
    ft_printf("%2.2s ", string + 8);
    if ((now - data->time) > SIXMONTHS || (time_t)now < data->time)
        ft_printf("%5.*s ", ft_nblen(ft_strtim_return(string + 20)), \
			ft_strtim_return(string + 20));
    else
        ft_printf("%5.5s ", string + 11);
}

void    print_files(t_data *data, unsigned flag)
{
    if (flag & FLAG_L)
    {
        print_permissions(data);
        ft_printf(" %*d ", data->w_st_nlink, data->stats.st_nlink);
        ft_printf("%-*s", data->w_pw_name, data->pw_name);
        ft_printf(" %-*s ", data->w_gr_name, data->gr_name);
        ft_printf(" %*lld ", data->w_st_nlink, data->stats.st_size);
        ft_print_date(data);
    }
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
    if (S_ISDIR(data->st_mode) && (!is_hidden(data->filename) || (data->args > 1 && !data->level)))
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