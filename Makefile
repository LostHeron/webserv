# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/01 16:54:22 by jweber            #+#    #+#              #
#    Updated: 2026/04/10 16:56:48 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -MMD -MP -std="c++98"

INCLUDES = -I includes\
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SERVER_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(LISTEN_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(EPOLLSTRUCT_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(AFD_DIR) \

AFD_DIR := AFd/
AFD_FILES := AFd.cpp \
			ListenFd.cpp \
			IOFd.cpp \

LISTEN_DIR := Listen/
LISTEN_FILES := Listen.cpp \

EPOLLSTRUCT_DIR := EpollStruct/
EPOLLSTRUCT_FILES := EpollStruct.cpp \

SERVER_DIR := Server/
SERVER_FILES := Server.cpp \

CLASSES_DIR := classes/
CLASSES_FILES := $(addprefix $(SERVER_DIR), $(SERVER_FILES)) \
				 $(addprefix $(LISTEN_DIR), $(LISTEN_FILES)) \
				 $(addprefix $(EPOLLSTRUCT_DIR), $(EPOLLSTRUCT_FILES)) \
				 $(addprefix $(AFD_DIR), $(AFD_FILES)) \


SRCS_DIR := sources/
SRCS_FILES := $(addprefix $(SRCS_DIR), webserv.cpp) \
			  $(addprefix $(SRCS_DIR), $(addprefix $(CLASSES_DIR), $(CLASSES_FILES))) \

OBJ_DIR := .obj/
OBJECTS := $(addprefix $(OBJ_DIR), $(SRCS_FILES:.cpp=.o))

D_FILES := $(OBJECTS:.o=.d)

.PHONY: all clean fclean re
.DEFAULT_GOAL = all

echo:
	@echo $(CLASSES_FILES)


all: $(NAME)

-include $(D_FILES)

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
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:
	$(MAKE) clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all


debug:
	rm webserv
	$(MAKE) all CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -Wno-unused" OBJ_DIR=".obj_debug/"

debug_clean:
	$(MAKE) clean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3" OBJ_DIR=".obj_debug/"

debug_fclean:
	$(MAKE) fclean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3" OBJ_DIR=".obj_debug/"

debug_re:
	$(MAKE) re CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3" OBJ_DIR=".obj_debug/"
