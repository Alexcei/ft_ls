/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_del.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpole <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 20:10:58 by bpole             #+#    #+#             */
/*   Updated: 2019/11/15 20:11:21 by bpole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		free_node(t_avltree1 *node)
{
	if (node)
	{
		ft_avlt_free1(&(node->data->subtree));
		ft_memdel((void *)(&(node->data->width)));
		ft_memdel((void *)(&(node->data->filename)));
		ft_memdel((void *)(&(node->data->dir)));
		ft_memdel((void *)(&(node->data)));
		ft_memdel((void *)&node);
	}
}

static void		btree_apply_suffix(t_avltree1 *root, void (*f)(t_avltree1 *))
{
	if (root == NULL)
		return ;
	btree_apply_suffix(root->left, f);
	btree_apply_suffix(root->right, f);
	f(root);
}

void			ft_avlt_free1(t_avltree1 **root)
{
	if (root == NULL || *root == NULL)
		return ;
	btree_apply_suffix(*root, &free_node);
	*root = NULL;
}
