# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/13 23:20:17 by val               #+#    #+#              #
#    Updated: 2025/03/06 16:37:34 by vdurand          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
RESET = \033[0m
BG_GREEN = \033[42m

NAME = pipex

SRC_DIR = src
MAIN_DIR = src_main
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = libft

SRC = $(SRC_DIR)/pipex.c \
	$(SRC_DIR)/exec.c \
	$(SRC_DIR)/parsing.c
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
MAIN = $(MAIN_DIR)/main.c
MAIN_BONUS = $(MAIN_DIR)/main_bonus.c

ifeq ($(MAKECMDGOALS),bonus)
	MAIN = $(MAIN_DIR)/main_bonus.c
endif

MAIN_OBJ = $(OBJ_DIR)/$(notdir $(MAIN:.c=.o))

CC = cc
CFLAGS = -Werror -Wextra -Wall
FTFLAGS = -L$(LIBFT_DIR) -lft
LDFLAGS = $(FTFLAGS)
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)

all: makelibft $(NAME)

bonus: $(NAME)

$(NAME): $(OBJ) $(MAIN_OBJ) 
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "$(BG_GREEN)>>> Program $(NAME) compiled!$(RESET)"

makelibft:
	$(MAKE) -C $(LIBFT_DIR)

$(MAIN_OBJ): $(MAIN) 
	@echo "$(BLUE)>>> Compiling $(MAIN)...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $(MAIN) -o $(MAIN_OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile $(INC_DIR)/*.h $(LIBFT_DIR)/libft.a | $(OBJ_DIR) 
	@echo "$(BLUE)>>> Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@echo "$(YELLOW)>>> Directory '$(OBJ_DIR)' created!$(RESET)"
	@mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	@echo "$(BLUE)>>> Compiling Libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null 2>&1
	@$(MAKE) bonus -C $(LIBFT_DIR) > /dev/null 2>&1
	@echo "$(GREEN)>>> Compilation achieved!$(RESET)"

cleanlibs:
	@echo "$(YELLOW)>>> Cleaning libs...$(RESET)"
	@$(MAKE) fclean -C $(LIBFT_DIR) > /dev/null 2>&1

clean:
	@echo "$(YELLOW)>>> Cleaning objects$(RESET)"
	@rm -rf $(OBJ_DIR) > /dev/null 2>&1

fclean: clean cleanlibs
	@echo "$(YELLOW)>>> Cleaning executable...$(RESET)"
	@rm -f $(NAME) > /dev/null 2>&1

re: fclean all

.PHONY: all cleanlibs clean fclean re makelibft