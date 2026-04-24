# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cviel <cviel@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/01 16:54:22 by jweber            #+#    #+#              #
#    Updated: 2026/04/13 18:21:52 by cviel            ###   ########.fr        #
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
<<<<<<< HEAD
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(REQUEST_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(AMESSAGE_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(AFACTORY_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(GETREQ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(POSTREQ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(DELETEREQ_DIR) \
=======
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(JSONLEXER_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(JSONOBJ_DIR) \

>>>>>>> 947a2797646ddb2b39267fb7dff2eff773a0bd7a

AMESSAGE_DIR := 	AMessage/
AMESSAGE_FILES := 	AMessage.cpp \

<<<<<<< HEAD
AFACTORY_DIR := 	AFactory/
AFACTORY_FILES := 	AFactory.cpp \

REQUEST_DIR := 		ARequest/
REQUEST_FILES := 	ARequest.cpp \
				 	RequestFactory.cpp \

GETREQ_DIR := 		GETReq/
GETREQ_FILES := 	GETReq.cpp \

DELETEREQ_DIR := 	DELETEReq/
DELETEREQ_FILES := 	DELETEReq.cpp \

POSTREQ_DIR := 		POSTReq/
POSTREQ_FILES := 	POSTReq.cpp \

# AFD_DIR := 		AFd/
# AFD_FILES := 	AFd.cpp \
				ListenFd.cpp \
				IOFd.cpp \

# LISTEN_DIR := 	Listen/
# LISTEN_FILES :=	Listen.cpp \
#
# EPOLLSTRUCT_DIR := EpollStruct/
# EPOLLSTRUCT_FILES := EpollStruct.cpp \
#
# SERVER_DIR := Server/
# SERVER_FILES := Server.cpp \
=======
EPOLLSTRUCT_DIR := EpollStruct/
EPOLLSTRUCT_FILES := EpollStruct.cpp \
>>>>>>> 947a2797646ddb2b39267fb7dff2eff773a0bd7a


JSONLEXER_DIR := JsonLexer/
JSONLEXER_FILES := JsonLexer.cpp

JSONOBJ_DIR := JsonObj/
JSONOBJ_FILES := JsonObj.cpp

CLASSES_DIR := classes/
<<<<<<< HEAD
CLASSES_FILES :=	$(addprefix $(AMESSAGE_DIR), $(AMESSAGE_FILES)) \
					$(addprefix $(REQUEST_DIR), $(REQUEST_FILES)) \
					$(addprefix $(AFACTORY_DIR), $(AFACTORY_FILES)) \
					$(addprefix $(GETREQ_DIR), $(GETREQ_FILES)) \
					$(addprefix $(POSTREQ_DIR), $(POSTREQ_FILES)) \
					$(addprefix $(DELETEREQ_DIR), $(DELETEREQ_FILES)) \
					# $(addprefix $(SERVER_DIR), $(SERVER_FILES)) \
					# $(addprefix $(LISTEN_DIR), $(LISTEN_FILES)) \
					# $(addprefix $(EPOLLSTRUCT_DIR), $(EPOLLSTRUCT_FILES)) \
					# $(addprefix $(AFD_DIR), $(AFD_FILES)) \
=======
CLASSES_FILES := $(addprefix $(SERVER_DIR), $(SERVER_FILES)) \
				 $(addprefix $(LISTEN_DIR), $(LISTEN_FILES)) \
				 $(addprefix $(EPOLLSTRUCT_DIR), $(EPOLLSTRUCT_FILES)) \
				 $(addprefix $(AFD_DIR), $(AFD_FILES)) \
				 $(addprefix $(JSONLEXER_DIR), $(JSONLEXER_FILES)) \
				 $(addprefix $(JSONOBJ_DIR), $(JSONOBJ_FILES)) \
>>>>>>> 947a2797646ddb2b39267fb7dff2eff773a0bd7a

SOCKETS_DIR := sockets/
SOCKETS_FILES := CreateFd.cpp \
				 start.cpp \

CONFIGFILE_DIR := config_file/
CONFIGFILE_FILES := setup.cpp \

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
	# rm webserv
	$(MAKE) all CXX="c++" CXXFLAGS="$(CXXFLAGS) -g3 -Wno-unused" OBJ_DIR=".obj_debug/"

debug_clean:
	$(MAKE) clean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3" OBJ_DIR=".obj_debug/"

debug_fclean:
	$(MAKE) fclean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3" OBJ_DIR=".obj_debug/"

debug_re:
	$(MAKE) re CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3" OBJ_DIR=".obj_debug/"
