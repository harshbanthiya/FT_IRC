

CC := c++
CPPFLAGS := -Wall -Wextra -Werror -std=c++98
NAME :=	irc
SRCS :=	client.cpp Command_.cpp main_test.cpp server.cpp 
# Command_handler.cpp  
OBJS_DIR := object_files
OBJS := $(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))

all: $(NAME)
		@echo "Usage: ./irc  port password - To begin running the server"

$(OBJS_DIR)/%.o: %.cpp
		@mkdir -p $(OBJS_DIR)
		@$(CC) $(CPPFLAGS) -o $@ -c $^

$(NAME): $(OBJS)
		@$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)
		
clean:
		rm -rf $(OBJS_DIR) $(OBJS)

fclean:	clean
		rm -rf $(NAME)
	
re:			fclean all

.PHONY:	all clean fclean re
