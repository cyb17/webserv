# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yachen <yachen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/24 20:03:14 by yachen            #+#    #+#              #
#    Updated: 2024/06/25 10:45:19 by yachen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test

CC = c++

CFLAGS = -std=c++98 -Wall -Wextra -Werror

OBJ_DIR = obj

SRC_DIR = ./

SRC = main.cpp ConfigFile.cpp

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

DEPS = $(OBJ:%.o=%.d)

all: $(NAME)

$(NAME): $(OBJ)
		@echo "\e[32m\n==================================="
		@echo "\e[32mCompilin teste----------------"
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		@echo "\e[32m---created with success  teste"
		@echo "\e[32m===================================\n\e[0m"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "\e[32m\n====================="
	@echo "\e[32mCleanin teste----"
	@rm -rf $(OBJ_DIR)
	@echo "\e[32m--- teste Cleaned"
	@echo "\e[32m=====================\n\e[0m"

fclean:
	@echo "\e[32m\n====================="
	@echo "\e[32mCleanin teste----"
	@rm -rf $(OBJ_DIR)
	@echo "\e[32m--- teste Cleaned"
	@rm -rf $(NAME)
	@echo "\e[32m=====================\n\e[0m"

re: fclean all

-include $(DEPS)

.PHONY: clean fclean all re bonus