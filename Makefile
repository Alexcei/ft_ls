# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bpole <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/11 10:08:31 by bpole             #+#    #+#              #
#    Updated: 2019/11/15 11:51:14 by bpole            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

LIB_DIR = ./ft_printf/
LIBFT = $(LIB_DIR)libftprintf.a

SRC = ./main.c ./avl.c ./parse.c ./util.c ./out_put.c ./sort_args.c \
        ./get_data.c ./get_data_subtree.c ./out_put_permissions.c \
        ./avl_del.c ./balance_avl.c

OBJ = $(SRC:.c=.o)

INCLUDE = ./

CC = gcc

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

%.o: %.c
	$(CC) -c $(FLAGS) $<

$(NAME): $(OBJ)
	$(MAKE) -C $(LIB_DIR)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT)

clean:
	rm -rf $(OBJ)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all