#include "ft_ls.h"

static int     pars_flag(char **av, unsigned *flag)
{
    while (**av)
    {
        if (**av == 'l')
            *flag |= 1u;
        else if (**av == 'R')
            *flag |= 1u << 1u;
        else if (**av == 'a')
            *flag |= 1u << 2u;
        else if (**av == 'r')
            *flag |= 1u << 3u;
        else if (**av == 't')
            *flag |= 1u << 4u;
        else
        {
            ft_fprintf(2, "ls: illegal option -- %c\n", **av);
            ft_fprintf(2, "usage: ls [-Ralrst1] [file ...]\n");
            return (-1);
        }
        (*av)++;
    }
    return (0);
}

int     ft_ls_parse(int *ac, char ***av, unsigned *flag)
{
    if (**av && *av[0][0] == '-')
    {
        (**av)++;
        if ((pars_flag(*av, flag) == -1))
            return (1);
        (*av)++;
        (*ac)--;
    }
    return (0);
}