cmake_minimum_required(VERSION 3.14)
project(another_one_more_fd_ls_avltree C)

file(GLOB FILES
        "*.h"
        "*.c"
        "ft_printflibft/*.h"
        "ft_printf/libft/*.c"
        "ft_printf/*.h"
        "ft_printf/*.c")
        #"avl_tree/*.h"
        #"avl_tree/*.c")

set(CMAKE_C_STANDARD 99)
add_definitions(-Wall -Wextra -Werror)
add_executable(another_one_more_fd_ls_avltree ${FILES} ft_ls.h parse.c)