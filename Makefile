# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cviel <cviel@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/01 16:54:22 by jweber            #+#    #+#              #
#    Updated: 2026/05/19 16:38:16 by cviel            ###   ########.fr        #
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
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(JSONLEXER_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(JSONOBJ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(HOSTLIST_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(VIRTUALHOST_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(ASCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(BOOLSCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(INTSCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(STRINGSCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(OBJSCHEMA_DIR) \

AFD_DIR := AFd/
AFD_FILES := AFd.cpp \
			ListenFd.cpp \
			IOFd.cpp \

EPOLLSTRUCT_DIR := EpollStruct/
EPOLLSTRUCT_FILES := EpollStruct.cpp \

SERVER_DIR := Server/
SERVER_FILES := Server.cpp \

JSONLEXER_DIR := JsonLexer/
JSONLEXER_FILES := JsonLexer.cpp

JSONOBJ_DIR := JsonObj/
JSONOBJ_FILES := JsonObj.cpp

HOSTLIST_DIR := HostList/
HOSTLIST_FILES := HostList.cpp

VIRTUALHOST_DIR := VirtualHost/
VIRTUALHOST_FILES := VirtualHost.cpp

ASCHEMA_DIR := Schemas/ASchema/
ASCHEMA_FILES := ASchema.cpp

BOOLSCHEMA_DIR := Schemas/BoolSchema/
BOOLSCHEMA_FILES := BoolSchema.cpp

INTSCHEMA_DIR := Schemas/IntSchema/
INTSCHEMA_FILES := IntSchema.cpp

STRINGSCHEMA_DIR := Schemas/StringSchema/
STRINGSCHEMA_FILES := StringSchema.cpp

OBJSCHEMA_DIR := Schemas/ObjSchema/
OBJSCHEMA_FILES := ObjSchema.cpp

CLASSES_DIR := classes/
CLASSES_FILES := $(addprefix $(SERVER_DIR), $(SERVER_FILES)) \
				 $(addprefix $(LISTEN_DIR), $(LISTEN_FILES)) \
				 $(addprefix $(EPOLLSTRUCT_DIR), $(EPOLLSTRUCT_FILES)) \
				 $(addprefix $(AFD_DIR), $(AFD_FILES)) \
				 $(addprefix $(JSONLEXER_DIR), $(JSONLEXER_FILES)) \
				 $(addprefix $(JSONOBJ_DIR), $(JSONOBJ_FILES)) \
				 $(addprefix $(HOSTLIST_DIR), $(HOSTLIST_FILES)) \
				 $(addprefix $(VIRTUALHOST_DIR), $(VIRTUALHOST_FILES)) \
				 $(addprefix $(ASCHEMA_DIR), $(ASCHEMA_FILES)) \
				 $(addprefix $(BOOLSCHEMA_DIR), $(BOOLSCHEMA_FILES)) \
				 $(addprefix $(INTSCHEMA_DIR), $(INTSCHEMA_FILES)) \
				 $(addprefix $(STRINGSCHEMA_DIR), $(STRINGSCHEMA_FILES)) \
				 $(addprefix $(OBJSCHEMA_DIR), $(OBJSCHEMA_FILES)) \

SOCKETS_DIR := sockets/
SOCKETS_FILES := CreateFd.cpp \
				 start.cpp \

CONFIGFILE_DIR := config_file/
CONFIGFILE_FILES := schema_builders.cpp \
					string_validators.cpp \

SRCS_DIR := sources/
SRCS_FILES := webserv.cpp \
			  $(addprefix $(SOCKETS_DIR), $(SOCKETS_FILES)) \
			  $(addprefix $(CONFIGFILE_DIR), $(CONFIGFILE_FILES)) \
			  $(addprefix $(CLASSES_DIR), $(CLASSES_FILES)) \

SRCS_FILES := $(addprefix $(SRCS_DIR), $(SRCS_FILES))

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
