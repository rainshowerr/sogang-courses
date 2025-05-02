NAME	=	mp1_20200562

CC		=	gcc

SRCS	=	main.c

OBJS	=	$(SRCS:%.c=%.o)

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CC) -o $@ $^

%.o		:	%.c
			$(CC) -c $< -o $@

clean	:   
			rm -f $(OBJS)
			
fclean	:	clean
			rm -f $(NAME)

re		:	fclean
			make all
			
.PHONY	:	all clean fclean re