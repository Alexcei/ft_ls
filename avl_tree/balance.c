#include "avltree.h"

static int         bfactor(t_avltree* p)
{
    return (height(p->right) - height(p->left));
}

static void            fixheight(t_avltree* p)
{
    unsigned char hl;
    unsigned char hr;

    hl = height(p->left);
    hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

static t_avltree* rotateright(t_avltree* r)
{
    t_avltree* l;

    l = r->left;
    r->left = l->right;
    l->right = r;
    fixheight(r);
    fixheight(l);
    return (l);
}

static t_avltree* rotateleft(t_avltree* l)
{
    t_avltree* r;

    r = l->right;
    l->right = r->left;
    r->left = l;
    fixheight(l);
    fixheight(r);
    return (r);
}

t_avltree* balance(t_avltree* p)
{
    fixheight(p);
    if ((bfactor(p) == 2))
    {
        if (bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
            p->left = rotateleft(p->left);
        return (rotateright(p));
    }
    return (p);
}