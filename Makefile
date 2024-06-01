NAME = pipex
CC = cc
# CFLAGS = -Wall -Werror -Wextra
SRCS = main.c pipex_parsing.c utils/ft_putstr_fd.c utils/ft_split.c utils/ft_strjoin.c error_helper.c
HEADER = pipex.h
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

re : fclean all

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)
