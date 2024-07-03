# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 04:26:01 by hel-asli          #+#    #+#              #
#    Updated: 2024/07/03 02:54:36 by hel-asli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -Wall -Werror -Wextra
# LDFLAGS = -fsanitize=address -g


MANDATORY_DIR = Mandatory
BONUS_DIR = Bonus

# Source files
SRCS = $(MANDATORY_DIR)/main.c $(MANDATORY_DIR)/pipex_parsing.c \
       $(MANDATORY_DIR)/utils/ft_putstr_fd.c $(MANDATORY_DIR)/utils/ft_split.c \
       $(MANDATORY_DIR)/utils/ft_strjoin.c $(MANDATORY_DIR)/pipex_helper.c

BONUS_SRCS = $(BONUS_DIR)/main.c $(BONUS_DIR)/utils_bonus/ft_putstr_fd.c $(BONUS_DIR)/utils_bonus/ft_split.c \
             $(BONUS_DIR)/utils_bonus/ft_strjoin.c $(BONUS_DIR)/utils_bonus/ft_strcmp.c \
			 $(BONUS_DIR)/parsing.c $(BONUS_DIR)/get_next_line.c $(BONUS_DIR)/get_next_line_utils.c \
			 $(BONUS_DIR)/heredoc.c $(BONUS_DIR)/utils_bonus/ft_itoa.c $(BONUS_DIR)/pipex_helper.c \
			 $(BONUS_DIR)/pipex_cmds.c $(BONUS_DIR)/heredoc_helper.c $(BONUS_DIR)/pipex_init.c


OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)


HEADER = $(MANDATORY_DIR)/pipex.h
BONUS_HEADER = $(BONUS_DIR)/pipex_bonus.h


all: $(MANDATORY_DIR)/$(NAME)

bonus : $(BONUS_DIR)/$(NAME)

$(MANDATORY_DIR)/$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BONUS_DIR)/$(NAME) : $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(MANDATORY_DIR)/%.o: $(MANDATORY_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c $(BONUS_HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(MANDATORY_DIR)/$(NAME) $(BONUS_DIR)/$(NAME)

re: fclean all bonus

.PHONY: clean
