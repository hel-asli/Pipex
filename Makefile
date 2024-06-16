# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 04:26:01 by hel-asli          #+#    #+#              #
#    Updated: 2024/06/16 05:08:45 by hel-asli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the executable files
NAME = Mandatory/pipex
BONUS_NAME = Bonus/pipex

# Compiler and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra
# LDFLAGS = -fsanitize=address -g

# Source files
SRCS = Mandatory/main.c Mandatory/pipex_parsing.c \
       Mandatory/utils/ft_putstr_fd.c Mandatory/utils/ft_split.c \
       Mandatory/utils/ft_strjoin.c Mandatory/pipex_helper.c
BONUS_SRCS = Bonus/main.c Bonus/utils_bonus/ft_putstr_fd.c Bonus/utils_bonus/ft_split.c \
             Bonus/utils_bonus/ft_strjoin.c Bonus/utils_bonus/ft_strcmp.c

# Object files
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# Header files
HEADER = Mandatory/pipex.h
BONUS_HEADER = Bonus/pipex_bonus.h

# Targets
all: $(NAME)

bonus: $(BONUS_NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

Mandatory/%.o: Mandatory/%.c $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

Bonus/%.o: Bonus/%.c $(BONUS_HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all bonus

.PHONY: all bonus clean fclean re