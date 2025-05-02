NAME	=	mp3_20200562

CC		=	g++ -std=c++11

SRCS	=	main.cpp incoding.cpp decoding.cpp

OBJS	=	$(SRCS:%.cpp=%.o)

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CC) -o $@ $^

%.o		:	%.cpp
			$(CC) -c $< -o $@

clean	:   
			rm -f $(OBJS)
			
fclean	:	clean
			rm -f $(NAME)

re		:	fclean
			make all
			
.PHONY	:	all clean fclean re