NAME = pipex
CC = cc
CFLAGS = -Wall -Werror -Wextra
#LDFLAGS = -fsanitize=address -g
SRCS = Mandatory/main.c Mandatory/pipex_parsing.c\
	   Mandatory/utils/ft_putstr_fd.c Mandatory/utils/ft_split.c\
	   Mandatory/utils/ft_strjoin.c Mandatory/pipex_helper.c
	
HEADER = Mandatory/pipex.h
M_OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(M_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ 

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

re : fclean all

clean :
	$(RM) $(M_OBJS)

fclean : clean
	$(RM) $(NAME)

.PHONY : clean
