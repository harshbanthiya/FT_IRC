# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 12:14:47 by hbanthiy          #+#    #+#              #
#    Updated: 2022/08/21 19:02:11 by olabrecq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := c++
CPPFLAGS := -std=c++98 -g
ARGUMENTS = 6667 password
NAME :=	irc
SRCS :=	client.cpp Command.cpp main_test.cpp server.cpp replies.cpp command_user.cpp Utils.cpp
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

debugflags: 
	$(eval CPPFLAGS=-D DEBUG)

debug: debugflags run

clean:
		rm -rf $(OBJS_DIR) $(OBJS)

fclean:	clean
		rm -rf $(NAME)
	
re:			fclean all

.PHONY:	all clean fclean re debugflags debug
