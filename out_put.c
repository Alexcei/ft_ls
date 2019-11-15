#include "ft_ls.h"

static int	len_year(char *str)
{
    int len;

    len = 0;
    while (*str && *str++ != '\n')
        len++;
    return (len);
}

static int	get_year(char *str)
{
    int start;

    start = 0;
    while (++start && *str && *str == ' ')
        str++;
    return (start);
}

static void		ft_print_date(t_data *data)
{
    char	*str;
    char    *year;
    time_t	now;
    int     len;

    now = time(NULL);
    str = ctime(&(data->time));
    ft_printf("%3.3s ", str + 4);
    ft_printf("%2.2s ", str + 8);
    year = str + 19 + get_year(str + 20);
    len = len_year(year);
    if ((now - data->time) > (365 / 2) * 86400 || now < data->time)
        ft_printf(" %.*s ", len, year);
    else
        ft_printf("%5.5s ", str + 11);
}

void    print_flag_l(t_data *data, unsigned *flag)
{
    char *tmp;

    if (*flag & FLAG_L)
    {
        ft_output_permissions(data);
        if (listxattr(data->dir, NULL, 0, XATTR_NOFOLLOW) > 0)
            ft_printf("@");
        else
            ft_printf(" ");
        ft_printf("%*d ", data->width->w_st_nlink, data->stats.st_nlink);
        ft_printf("%-*s ", data->width->w_pw_name, data->pw_name);
        ft_printf(" %-*s ", data->width->w_gr_name, data->gr_name);
        ft_printf(" %*lld ", data->width->w_st_size, data->stats.st_size);
        ft_print_date(data);
    }
    if (S_ISLNK(data->st_mode) && *flag & FLAG_L)
    {
        tmp = ft_strnew(PATH_MAX);
        readlink(data->dir, tmp, PATH_MAX - 1);
        ft_printf("%s -> %s\n", data->filename, tmp);
        free(tmp);
    }
    else
        ft_printf("%s\n", data->filename);
}