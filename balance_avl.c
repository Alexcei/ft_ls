/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   balance_avl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpole <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 19:44:58 by bpole             #+#    #+#             */
/*   Updated: 2019/11/15 19:47:22 by bpole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int			bfactor(t_avltree1 *p)
{
	return (height1(p->right) - height1(p->left));
}

static void			fixheight(t_avltree1 *p)
{
	unsigned char	hl;
	unsigned char	hr;

	hl = height1(p->left);
	hr = height1(p->right);
	p->height1 = (hl > hr ? hl : hr) + 1;
}

static t_avltree1	*rotateright(t_avltree1 *r)
{
	t_avltree1		*l;

	l = r->left;
	r->left = l->right;
	l->right = r;
	fixheight(r);
	fixheight(l);
	return (l);
}

static t_avltree1	*rotateleft(t_avltree1 *l)
{
	t_avltree1		*r;

	r = l->right;
	l->right = r->left;
	r->left = l;
	fixheight(l);
	fixheight(r);
	return (r);
}

t_avltree1			*balance1(t_avltree1 *p)
{
	fixheight(p);
	if ((bfactor(p) == 2))
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return (rotateleft(p));
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return (rotateright(p));
	}
	return (p);
}
