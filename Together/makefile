# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yachen <yachen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/24 20:03:14 by yachen            #+#    #+#              #
#    Updated: 2024/07/10 15:18:15 by yachen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Server

CC = c++

CFLAGS = -std=c++98 -Wall -Wextra -Werror

OBJ_DIR = obj

SRC_DIR = ./

SRC = main.cpp ConfigParser.cpp ConfigExtractor.cpp HttpServer.cpp Request.cpp Response.cpp ResponseTools.cpp

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

DEPS = $(OBJ:%.o=%.d)

all: $(NAME)

$(NAME): $(OBJ)
		@echo "\e[32m\n==================================="
		@echo "\e[32mCompilin Server----------------"
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		@echo "\e[32m---created with success  Server"
		@echo "\e[32m===================================\n\e[0m"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "\e[32m\n====================="
	@echo "\e[32mCleanin Server----"
	@rm -rf $(OBJ_DIR)
	@echo "\e[32m--- Server Cleaned"
	@echo "\e[32m=====================\n\e[0m"

fclean:
	@echo "\e[32m\n====================="
	@echo "\e[32mCleanin Server----"
	@rm -rf $(OBJ_DIR)
	@echo "\e[32m--- Server Cleaned"
	@rm -rf $(NAME)
	@echo "\e[32m=====================\n\e[0m"

re: fclean all

-include $(DEPS)

.PHONY: clean fclean all re bonus