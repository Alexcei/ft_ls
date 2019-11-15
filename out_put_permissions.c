/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_put_permissions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpole <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 18:42:42 by bpole             #+#    #+#             */
/*   Updated: 2019/11/15 18:43:06 by bpole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		out_put_type(t_data *data)
{
	if (S_ISDIR(data->st_mode))
		ft_printf("d");
	else if (S_ISBLK(data->st_mode))
		ft_printf("b");
	else if (S_ISCHR(data->st_mode))
		ft_printf("c");
	else if (S_ISLNK(data->st_mode))
		ft_printf("l");
	else if (S_ISSOCK(data->st_mode))
		ft_printf("s");
	else if (S_ISREG(data->st_mode))
		ft_printf("-");
	else
		ft_printf("-");
}

void			ft_output_permissions(t_data *data)
{
	out_put_type(data);
	ft_printf((data->st_mode & S_IRUSR) ? "r" : "-");
	ft_printf((data->st_mode & S_IWUSR) ? "w" : "-");
	if (data->st_mode & S_ISUID)
		ft_printf((data->st_mode & S_IXUSR) ? "s" : "S");
	else
		ft_printf((data->st_mode & S_IXUSR) ? "x" : "-");
	ft_printf((data->st_mode & S_IRGRP) ? "r" : "-");
	ft_printf((data->st_mode & S_IWGRP) ? "w" : "-");
	if (data->st_mode & S_ISGID)
		ft_printf((data->st_mode & S_IXGRP) ? "s" : "S");
	else
		ft_printf((data->st_mode & S_IXGRP) ? "x" : "-");
	ft_printf((data->st_mode & S_IROTH) ? "r" : "-");
	ft_printf((data->st_mode & S_IWOTH) ? "w" : "-");
	if (data->st_mode & S_ISVTX)
		ft_printf((data->st_mode & S_IXUSR) ? "t" : "T");
	else
		ft_printf((data->st_mode & S_IXOTH) ? "x" : "-");
}
