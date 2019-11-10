#ifndef FT_LS_H
# define FT_LS_H

# include "ft_printf/libft/libft.h"
# include "ft_printf/ft_printf.h"
//# include "avl_tree/avltree.h"
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <dirent.h>
# include <sys/syslimits.h>
# include <time.h>
# include <errno.h>
# include <stdio.h>

# define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

# define FLAG_L (1u << 0u)
# define FLAG_R_UP (1u << 1u)
# define FLAG_A (1u << 2u)
# define FLAG_R (1u << 3u)
# define FLAG_T (1u << 4u)
# define FLAG_ONE (1u << 5u)

typedef struct s_avltree1   t_avltree1;
typedef struct s_data   t_data;

struct		s_data
{
    char			*dir;
    char			*filename;
    struct stat		stats;
    unsigned long	st_mode;
    unsigned	    flag;
    time_t			sec;
    time_t			time;
    int             w_st_nlink;
    int             w_pw_name;
    int             w_gr_name;
    //int             w_st_size;
    int				level;
    int             args;
    int             count;
    unsigned long	total_size;
    unsigned long	nb_files;  //??
    char			*pw_name;
    char			*gr_name;
    t_avltree1		*subtree;
}					;

struct            s_avltree1
{
    t_data                *data;
    unsigned char         height1;
    t_avltree1      *left;
    t_avltree1      *right;
}                         ;

void		      ft_avlt_free1(t_avltree1 **root);
void              ft_avlt_insert1(t_avltree1 **root, t_data *data, int (*cmp)(t_data *,t_data *));
unsigned char     height1(t_avltree1* p);
t_avltree1        *balance1(t_avltree1* p);

int     ft_ls_parse(int *ac, char ***av, unsigned *flag);
void    btree_apply_infix(t_avltree1 *root, void (*applyf)(t_data*, unsigned i), unsigned flag);

int         ft_name_cmp(t_data *data1, t_data *data2);
int         ft_date_cmp(t_data *data1, t_data *data2);
int	        is_hidden(char *name);

#endif