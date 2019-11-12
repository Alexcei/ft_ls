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
        else if (**av == '1')
            *flag |= 1u << 5u;
        else if (**av == '-')
            return (1);
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
    int     ret;

    while (**av && av[0][0][0] == '-' && av[0][0][1])
    {
        (**av)++;
        ret = pars_flag(*av, flag);
        if (ret == -1)
            return (1);
        else if (ret == 1)
        {
            (*av)++;
            (*ac)--;
            break;
        }
        (*av)++;
        (*ac)--;
    }
    //if (*flag & FLAG_ONE)
      //  *flag ^= 1u;
    return (0);
}