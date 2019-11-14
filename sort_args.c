#include "ft_ls.h"

static void     ft_swap_argv(char **av1, char **av2)
{
    char    *tmp;
    tmp = *av1;
    *av1 = *av2;
    *av2 = tmp;
}

static int      ft_arg_cmp(char *av1, char *av2)
{
    return (ft_strcmp(av1, av2));
}

char    **ft_sort_args(char **av, int ac)
{
    char **argv;
    int flag;
    int i;

    i = -1;
    flag = 1;
    argv = (char**)ft_memalloc(sizeof(char*) * ac + 1);
    while (++i < ac)
        argv[i] = av[i];
    while (flag)
    {
        i = 0;
        flag = 0;
        while (i < ac - 1)
        {
            if (ft_arg_cmp(argv[i], argv[i + 1]) >= 0 && (flag = 1))
                ft_swap_argv(argv + i, argv + i + 1);
            i++;
        }
        ac--;
    }
    return (argv);
}