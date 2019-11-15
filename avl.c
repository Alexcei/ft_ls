/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpole <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 18:46:21 by bpole             #+#    #+#             */
/*   Updated: 2019/11/15 18:48:20 by bpole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

unsigned char		height1(t_avltree1 *p)
{
	return (p ? p->height1 : 0);
}

static t_avltree1	*ft_avlt_create(t_data *data)
{
	t_avltree1		*node;
	t_data			*data_cpy;

	node = (t_avltree1 *)ft_memalloc(sizeof(t_avltree1));
	if (node == NULL)
		return (NULL);
	data_cpy = (t_data *)ft_memalloc(sizeof(t_data));
	if (data_cpy == NULL)
	{
		ft_memdel((void *)&node);
		return (NULL);
	}
	ft_memcpy(data_cpy, data, sizeof(t_data));
	node->data = data_cpy;
	node->height1 = 1;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void				ft_avlt_insert1(t_avltree1 **root, t_data *data,
					int (*cmp)(t_data *, t_data *))
{
	if (!*root || !(*root)->data)
	{
		*root = ft_avlt_create(data);
	}
	else
	{
		if (cmp((*root)->data, data) > 0)
			ft_avlt_insert1(&((*root)->left), data, cmp);
		else
			ft_avlt_insert1(&((*root)->right), data, cmp);
		*root = balance1(*root);
	}
}
