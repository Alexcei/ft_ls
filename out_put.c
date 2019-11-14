#include "ft_ls.h"

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

void		ft_print_date(t_data *data)
{
    char	*string;
    time_t	now;

    now = time(NULL);
    string = ctime((time_t *)&(data->time));
    ft_printf("%3.3s ", string + 4);
    ft_printf("%2.2s ", string + 8);
    if ((now - data->time) > (365 / 2) * 86400 || (time_t)now < data->time)
        ft_printf(" %.*s ", ft_nblen(ft_strtim_return(string + 20)), ft_strtim_return(string + 20));
    else
        ft_printf("%5.5s ", string + 11);
}

void	print_link(t_data *data)
{
    char *tmp;

    tmp = ft_strnew(PATH_MAX);
    readlink(data->dir, tmp, PATH_MAX - 1);
    ft_printf("%s -> %s\n", data->filename, tmp);
    free(tmp);
}

void    print_extended(t_data *data)
{
    if (listxattr(data->dir, NULL, 0, XATTR_NOFOLLOW) > 0)
        ft_printf("@");
    else
        ft_printf(" ");
}

void    print_files(t_data *data, unsigned *flag)
{
    if (*flag & FLAG_L)
    {
        print_permissions(data);
        print_extended(data);
        ft_printf("%*d ", data->width->w_st_nlink, data->stats.st_nlink);
        ft_printf("%-*s ", data->width->w_pw_name, data->pw_name);
        ft_printf(" %-*s ", data->width->w_gr_name, data->gr_name);
        ft_printf(" %*lld ", data->width->w_st_size, data->stats.st_size);
        ft_print_date(data);
    }
    if (S_ISLNK((data->stats).st_mode) && *flag & FLAG_L)
        print_link(data);
    else
        ft_printf("%s\n", data->filename);
}