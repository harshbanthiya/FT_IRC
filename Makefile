# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sheeed <sheeed@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 12:14:47 by hbanthiy          #+#    #+#              #
#    Updated: 2022/08/26 08:47:24 by sheeed           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := c++
CPPFLAGS := -Wall -Wextra -Werror -std=c++98
ARGUMENTS = 6667 password
NAME :=	irc
SRCS :=	client.cpp Command_Handler.cpp main.cpp server.cpp
OBJS_DIR := object_files
OBJS := $(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))

all: $(NAME)
		@echo "Usage: ./irc  port password - To begin running the server"

$(OBJS_DIR)/%.o: %.cpp
		@mkdir -p $(OBJS_DIR)
		@$(CC) $(CPPFLAGS) -o $@ -c $^

$(NAME): $(OBJS)
		@$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)
run: all 
	./$(NAME) $(ARGUMENTS)

clean:
		rm -rf $(OBJS_DIR) $(OBJS)

fclean:	clean
		rm -rf $(NAME)
	
re:			fclean all

.PHONY:	all clean fclean re debugflags debug
