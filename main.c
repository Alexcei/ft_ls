/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpole <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 18:35:57 by bpole             #+#    #+#             */
/*   Updated: 2019/11/15 19:40:10 by bpole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int		ft_creat_data(t_avltree1 **root,
				int ac, char **av, unsigned flag)
{
	t_data		*data;
	int			i;
	t_width		width;

	i = 0;
	ft_bzero(&width, sizeof(t_width));
	while (i < ac || !i)
	{
		if (ac && !av[i][0])
		{
			ft_fprintf(2, "ls: fts_open: No such file or directory\n");
			exit(1);
		}
		if (ac)
			data = ft_get_data(av[i], flag, 0, &width);
		else
			data = ft_get_data(".", flag, 0, &width);
		if (++i && !data)
			return (1);
		data->args = ac;
		ft_fill_tree_data(root, flag, data);
		btree_apply_infix_w(*root, ft_set_width, &width);
	}
	return (0);
}

static void		recursive(t_data *data, unsigned *flag)
{
	if (S_ISLNK(data->st_mode) && *flag & FLAG_R_UP && !ft_check_link(data))
		return ;
	if ((S_ISDIR(data->st_mode) && (!is_hidden(data->filename, data->flag)
			|| (data->args > 0 && !data->level))) || ft_check_link(data))
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
	btree_apply_infix(data->subtree, print_flag_l, flag);
	if (data->st_mode)
		*flag |= 1u << 10u;
	if (*flag & FLAG_R_UP)
		btree_apply_infix(data->subtree, recursive, flag);
}

static void		first_put(t_data *data, unsigned *flag)
{
	if (ft_check_link(data))
		return ;
	if (lstat(data->dir, &(data->stats)) == -1)
		return ;
	if (S_ISLNK((data->stats).st_mode) && *flag & FLAG_L)
		print_flag_l(data, flag);
	else if (!S_ISDIR(data->st_mode) && *flag & FLAG_L)
		print_flag_l(data, flag);
	else if (!S_ISDIR(data->st_mode))
	{
		if (!(S_ISLNK((data->stats).st_mode) && !data->level
					&& data->args == 1 && data->subtree))
		{
			ft_printf("%s\n", data->filename);
			*flag |= 1u << 10u;
		}
	}
}

int				main(int ac, char **av)
{
	t_avltree1	*root_content;
	unsigned	flag;
	char		**argv;

	ac--;
	av++;
	flag = 0;
	root_content = NULL;
	if (ft_ls_parse(&ac, &av, &flag))
		return (1);
	argv = ft_sort_args(av, ac);
	if (ft_creat_data(&root_content, ac, argv, flag) == 1)
	{
		ft_avlt_free1(&root_content);
		return (1);
	}
	btree_apply_infix(root_content, first_put, &flag);
	btree_apply_infix(root_content, recursive, &flag);
	ft_avlt_free1(&root_content);
	free(argv);
	return (0);
}
