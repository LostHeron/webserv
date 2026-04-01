# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/01 16:54:22 by jweber            #+#    #+#              #
#    Updated: 2026/04/01 17:48:14 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std="c++98"



SRCS_DIR := sources/
SRCS_FILES := $(addprefix $(SRCS_DIR), webserv.cpp) \

OBJ_DIR := .obj/
OBJECTS := $(addprefix $(OBJ_DIR), $(SRCS_FILES:.cpp=.o))

.PHONY: all clean fclean re
.DEFAULT_GOAL = all

echo:
	@echo $(OBJECTS)
	@echo $(SRCS_FILES)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)%.o:%.cpp
	@if ! [ -d $(shell echo $@ | sed "s/\/\w*\.o//g") ]; then\
		echo "directory does not exists, creating it!" ; \
		echo "mkdir -p " $(shell echo $@ | sed "s/\/\w*\.o//g") ; \
		mkdir -p $(shell echo $@ | sed "s/\/\w*\.o//g") ; \
	else \
		echo "directory does exists" ;\
	fi
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:
	$(MAKE) clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all
