#include "avltree.h"

unsigned char   height(t_avltree* p)
{
    return (p ? p->height : 0);
}