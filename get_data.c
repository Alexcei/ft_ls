#include "ft_ls.h"

static void    ft_get_values2(t_data *data, t_width *width)
{
    struct passwd	*pwd;
    struct group	*grp;

    if ((pwd = getpwuid((data->stats).st_uid)))
    {
        data->pw_name = pwd->pw_name;
        width->w_pw_name = MAX((int)ft_strlen(data->pw_name), width->w_pw_name);
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
}

static t_data	*ft_get_values(t_data *data, unsigned flag, int level, int count, t_width *width)
{
    data->level = level;
    data->count = count;
    data->flag = flag;
    data->time = (data->stats).st_mtime;
    if (!is_hidden(data->filename, data->flag) || (!data->level))
    {
        if (data->level < 2 || flag & FLAG_R_UP)
            data->subtree = ft_creat_subtree(data);
    }
    else
        data->subtree = NULL;
    ft_get_values2(data, width);
    return (data);
}

static char	*ft_get_filename(t_data *data)
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

static void    fill_width(t_data *data, t_width *width)
{
    int num_len;

    num_len = ft_fprintf(-1, "%lld", data->stats.st_nlink) + 1;
    width->w_st_nlink = MAX(num_len, width->w_st_nlink);
    num_len = ft_fprintf(-1, "%lld", (data->stats).st_size);
    width->w_st_size = MAX(num_len, width->w_st_size);
}

t_data			*ft_get_data(char *dir, unsigned flag, int level, int count, t_width *width)
{
    t_data	    *data;
    struct stat stats;
    DIR				*d;

    errno = 0;
    ft_bzero(&stats, sizeof(stats));
    if (lstat(dir, &stats) == -1)
        ft_fprintf(2, "ls: %s: %s\n", dir, strerror(errno));
    if (!(data = (t_data*)ft_memalloc(sizeof(t_data))))
        return (NULL);
    data->width = ft_memalloc(sizeof(t_width));
    data->stats = stats;
    data->st_mode = stats.st_mode;
    data->dir = ft_strdup(dir);
    data->filename = ft_get_filename(data);
    errno = 0;
    if (!(d = opendir(data->dir)) && S_ISDIR(data->st_mode) && (!level || flag & FLAG_R_UP))
        ft_fprintf(2,"ls: %s: %s\n", data->filename, strerror(errno));
    fill_width(data, width);
    if (!is_hidden(data->filename, data->flag) || (!data->count && !data->level))
        data = ft_get_values(data, flag, level, count, width);
    if (d)
        closedir(d);
    return (data);
}