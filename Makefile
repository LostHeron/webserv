# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/11 16:35:18 by jweber            #+#    #+#              #
#    Updated: 2026/07/02 13:43:45 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
NAME_SANITIZE := "webserv_no_valgrind"
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -MMD -MP -std="c++98"

INCLUDES = -I includes\
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SERVER_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(LISTEN_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(EPOLLSTRUCT_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(HTTPSTATUS_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(HTMLPAGEBUILDER_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(PIPE_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(HEADERSBUILDER_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(EXCEPTIONS_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(VIRTUALHOST_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(VHOSTLIST_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(ASOCKET_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(CONNECTION_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(CHUNK_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(ENVIRONMENT_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(REQUEST_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(AMESSAGE_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(RESPONSE_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(AFACTORY_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(COOKIE_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SESSION_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(REQUEST_DIR)$(GETREQ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(REQUEST_DIR)$(POSTREQ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(REQUEST_DIR)$(DELETEREQ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(REQUEST_DIR)$(UNKNOWNREQ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(JSONLEXER_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(JSONOBJ_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SCHEMA_DIR)$(ASCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SCHEMA_DIR)$(BOOLSCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SCHEMA_DIR)$(INTSCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SCHEMA_DIR)$(OBJSCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(SCHEMA_DIR)$(STRINGSCHEMA_DIR) \
		   -I $(SRCS_DIR)$(CLASSES_DIR)$(VHOSTPARSER_DIR) \

HTTPSTATUS_DIR :=	HTTPStatus/
HTTPSTATUS_FILES :=	HTTPStatus.cpp

SESSION_DIR :=		Session/
SESSION_FILES :=	Session.cpp

HTMLPAGEBUILDER_DIR :=		HTMLPageBuilder/
HTMLPAGEBUILDER_FILES :=	HTMLPageBuilder.cpp

SCHEMA_DIR :=		Schemas/

COOKIE_DIR :=	Cookie/
COOKIE_FILES :=	Cookie.cpp

ASCHEMA_DIR :=		ASchema/
ASCHEMA_FILES := 	ASchema.cpp

BOOLSCHEMA_DIR :=	BoolSchema/
BOOLSCHEMA_FILES := BoolSchema.cpp

INTSCHEMA_DIR :=	IntSchema/
INTSCHEMA_FILES := IntSchema.cpp

OBJSCHEMA_DIR :=	ObjSchema/
OBJSCHEMA_FILES := ObjSchema.cpp

STRINGSCHEMA_DIR :=	StringSchema/
STRINGSCHEMA_FILES := StringSchema.cpp
					

AMESSAGE_DIR := 	AMessage/
AMESSAGE_FILES := 	AMessage.cpp \

RESPONSE_DIR := 	Response/
RESPONSE_FILES := 	Response.cpp \
					
AFACTORY_DIR := 	AFactory/
AFACTORY_FILES := 	AFactory.cpp \

REQUEST_DIR := 		Request/
REQUEST_FILES := 	ARequest.cpp \
				 	RequestFactory.cpp \

GETREQ_DIR := 		GETReq/
GETREQ_FILES := 	GETReq.cpp \

DELETEREQ_DIR := 	DELETEReq/
DELETEREQ_FILES := 	DELETEReq.cpp \

POSTREQ_DIR := 		POSTReq/
POSTREQ_FILES := 	POSTReq.cpp \

UNKNOWNREQ_DIR := 	UNKNOWNReq/
UNKNOWNREQ_FILES := UNKNOWNReq.cpp \

CHUNK_DIR := Chunk/
CHUNK_FILES := Chunk.cpp \

CONNECTION_DIR := Connection/
CONNECTION_FILES := Connection.cpp \

ENVIRONMENT_DIR := Environment/
ENVIRONMENT_FILES := Environment.cpp \

ASOCKET_DIR := 	ASocket/
ASOCKET_FILES := 	ASocket.cpp \
					ListenSocket.cpp \
					InputSocket.cpp \
					InputSocket_process.cpp \
					InputSocket_process_method.cpp \
					InputSocket_process_uri.cpp \
					InputSocket_process_version.cpp \
					InputSocket_process_header.cpp \
					InputSocket_process_request.cpp \
					InCGI.cpp \
					OutCGI.cpp \
					OutputSocket.cpp \

PIPE_DIR := Pipe/
PIPE_FILES := Pipe.cpp \

HEADERSBUILDER_DIR := HeadersBuilder/
HEADERSBUILDER_FILES := HeadersBuilder.cpp \

EXCEPTIONS_DIR := Exceptions/
EXCEPTIONS_FILES := IsChildren.cpp \

SERVER_DIR := Server/
SERVER_FILES := Server.cpp \

EPOLLSTRUCT_DIR := EpollStruct/
EPOLLSTRUCT_FILES := EpollStruct.cpp \

JSONLEXER_DIR := JsonLexer/
JSONLEXER_FILES := JsonLexer.cpp

JSONOBJ_DIR := JsonObj/
JSONOBJ_FILES := JsonObj.cpp

VHOSTPARSER_DIR := VHostParser/
VHOSTPARSER_FILES := VHostParser.cpp

VHOSTLIST_DIR := VHostList/
VHOSTLIST_FILES := VHostList.cpp

VIRTUALHOST_DIR := VirtualHost/
VIRTUALHOST_FILES := VirtualHost.cpp


SIGNALS_HANDLING_DIR := signals/
SIGNALS_HANDLING_FILES := setup_signals.cpp \

ERROR_DIR := error/
ERROR_FILES := logerror.cpp \

CLASSES_DIR := classes/
CLASSES_FILES := $(addprefix $(SERVER_DIR), $(SERVER_FILES)) \
				 $(addprefix $(LISTEN_DIR), $(LISTEN_FILES)) \
				 $(addprefix $(AMESSAGE_DIR), $(AMESSAGE_FILES)) \
				 $(addprefix $(RESPONSE_DIR), $(RESPONSE_FILES)) \
				 $(addprefix $(REQUEST_DIR), $(REQUEST_FILES)) \
				 $(addprefix $(AFACTORY_DIR), $(AFACTORY_FILES)) \
				 $(addprefix $(REQUEST_DIR)$(GETREQ_DIR), $(GETREQ_FILES)) \
				 $(addprefix $(REQUEST_DIR)$(POSTREQ_DIR), $(POSTREQ_FILES)) \
				 $(addprefix $(REQUEST_DIR)$(DELETEREQ_DIR), $(DELETEREQ_FILES)) \
				 $(addprefix $(REQUEST_DIR)$(UNKNOWNREQ_DIR), $(UNKNOWNREQ_FILES)) \
				 $(addprefix $(EPOLLSTRUCT_DIR), $(EPOLLSTRUCT_FILES)) \
				 $(addprefix $(PIPE_DIR), $(PIPE_FILES)) \
				 $(addprefix $(EXCEPTIONS_DIR), $(EXCEPTIONS_FILES)) \
				 $(addprefix $(ASOCKET_DIR), $(ASOCKET_FILES)) \
				 $(addprefix $(CONNECTION_DIR), $(CONNECTION_FILES)) \
				 $(addprefix $(CHUNK_DIR), $(CHUNK_FILES)) \
				 $(addprefix $(ENVIRONMENT_DIR), $(ENVIRONMENT_FILES)) \
				 $(addprefix $(JSONLEXER_DIR), $(JSONLEXER_FILES)) \
				 $(addprefix $(COOKIE_DIR), $(COOKIE_FILES)) \
				 $(addprefix $(SESSION_DIR), $(SESSION_FILES)) \
				 $(addprefix $(JSONOBJ_DIR), $(JSONOBJ_FILES)) \
				 $(addprefix $(HTTPSTATUS_DIR), $(HTTPSTATUS_FILES)) \
				 $(addprefix $(HTMLPAGEBUILDER_DIR), $(HTMLPAGEBUILDER_FILES)) \
				 $(addprefix $(HEADERSBUILDER_DIR), $(HEADERSBUILDER_FILES)) \
				 $(addprefix $(VHOSTLIST_DIR), $(VHOSTLIST_FILES)) \
				 $(addprefix $(VIRTUALHOST_DIR), $(VIRTUALHOST_FILES)) \
				 $(addprefix $(VHOSTPARSER_DIR), $(VHOSTPARSER_FILES)) \
				 $(addprefix $(SCHEMA_DIR)$(ASCHEMA_DIR), $(ASCHEMA_FILES)) \
				 $(addprefix $(SCHEMA_DIR)$(BOOLSCHEMA_DIR), $(BOOLSCHEMA_FILES)) \
				 $(addprefix $(SCHEMA_DIR)$(INTSCHEMA_DIR), $(INTSCHEMA_FILES)) \
				 $(addprefix $(SCHEMA_DIR)$(STRINGSCHEMA_DIR), $(STRINGSCHEMA_FILES)) \
				 $(addprefix $(SCHEMA_DIR)$(OBJSCHEMA_DIR), $(OBJSCHEMA_FILES)) \

SOCKETS_DIR := sockets/
SOCKETS_FILES := CreateFd.cpp \
				 start.cpp \

SIGNALS_HANDLING_DIR := signals/
SIGNALS_HANDLING_FILES := setup_signals.cpp \

CONFIGFILE_DIR := config_file/
CONFIGFILE_FILES := schema_builders.cpp \
					string_validators.cpp \

SRCS_DIR := sources/
SRCS_FILES := webserv.cpp \
			  $(addprefix $(SOCKETS_DIR), $(SOCKETS_FILES)) \
			  $(addprefix $(SIGNALS_HANDLING_DIR), $(SIGNALS_HANDLING_FILES)) \
			  $(addprefix $(ERROR_DIR), $(ERROR_FILES)) \
			  $(addprefix $(CLASSES_DIR), $(CLASSES_FILES))	\
			  $(addprefix $(CONFIGFILE_DIR), $(CONFIGFILE_FILES)) \

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
	rm -f $(NAME_SANITIZE)
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
	rm -f $(NAME_SANITIZE)

re:
	$(MAKE) fclean
	$(MAKE) all


debug_info:
	rm -f $(NAME)
	rm -f $(NAME_SANITIZE)
	$(MAKE) all CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -D INFO_WEBSERV" OBJ_DIR=".obj_debug_info/"

debug_info_clean:
	$(MAKE) clean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -D INFO_WEBSERV" OBJ_DIR=".obj_debug_info/"

debug_info_fclean:
	$(MAKE) fclean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -D INFO_WEBSERV" OBJ_DIR=".obj_debug_info/"

debug_info_re:
	$(MAKE) re CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -D INFO_WEBSERV" OBJ_DIR=".obj_debug_info/"

debug_print:
	rm -f $(NAME)
	rm -f $(NAME_SANITIZE)
	$(MAKE) all CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -Wno-unused -D DEBUG" OBJ_DIR=".obj_debug_print/"

debug_print_clean:
	$(MAKE) clean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -Wno-unused -D DEBUG" OBJ_DIR=".obj_debug_print/"

debug_print_fclean:
	$(MAKE) fclean CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -Wno-unused -D DEBUG" OBJ_DIR=".obj_debug_print/"

debug_print_re:
	$(MAKE) re CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -Wno-unused -D DEBUG" OBJ_DIR=".obj_debug_print/"

debug_sanitize:
	rm -f $(NAME)
	rm -f $(NAME_SANITIZE)
	$(MAKE) all NAME="$(NAME_SANITIZE)" CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -fsanitize=address -Wno-unused -D INFO_WEBSERV" OBJ_DIR=".obj_debug_sanitize/"

debug_sanitize_clean:
	$(MAKE) clean NAME="$(NAME_SANITIZE)" CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -fsanitize=address -Wno-unused -D INFO_WEBSERV" OBJ_DIR=".obj_debug_sanitize/"

debug_sanitize_fclean:
	$(MAKE) fclean NAME="$(NAME_SANITIZE)" CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -fsanitize=address -Wno-unused -D INFO_WEBSERV" OBJ_DIR=".obj_debug_sanitize/"

debug_sanitize_re:
	$(MAKE) re NAME="$(NAME_SANITIZE)" CXX="g++" CXXFLAGS="$(CXXFLAGS) -g3 -fsanitize=address -Wno-unused -D INFO_WEBSERV" OBJ_DIR=".obj_debug_sanitize/"



print-%:
	@echo $($(patsubst print-%,%,$@))
