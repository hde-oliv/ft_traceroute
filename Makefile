INC_DIR :=	headers
SRC_DIR :=	sources
OBJ_DIR :=	objects

INCS	:=	$(INC_DIR)
SRCS	:=	$(shell find $(SRC_DIR) -name '*.c')
OBJS    :=	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC		:=	clang
NAME	:=	ft_traceroute

CFLAGS	+=	-Wall -Wextra -Werror
LFLAGS  +=	-I.
TFLAGS	+=	-fsanitize=address -g
RM		:=	rm -rf


all:		obj $(NAME)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
			$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS)/$(INCS)

$(NAME):	$(OBJS)
			$(CC) -lm -o $(NAME) $(OBJS)

obj:
			@mkdir -p $(OBJ_DIR)

clean:
			$(RM) $(OBJS) $(OBJ_DIR)

fclean: 	clean
			$(RM) $(NAME)

re: 		fclean all

.PHONY: 	all clean fclean re obj
