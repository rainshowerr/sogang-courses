NAME	=	mp2_20200562

CC		=	gcc

SRCS	=	main.c selection_sort.c quick_sort.c merge_sort.c my_sort.c util.c

OBJS	=	$(SRCS:%.c=%.o)

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CC) -o $@ $^ -lm

%.o		:	%.c
			$(CC) -c $< -o $@

clean	:   
			rm -f $(OBJS)
			
fclean	:	clean
			rm -f $(NAME)

re		:	fclean
			make all
			
.PHONY	:	all clean fclean re