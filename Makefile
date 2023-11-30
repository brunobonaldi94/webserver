# ==============================================================================
# VARIABLES
# ==============================================================================
NAME			:=	webserv
NAME_TEST	:= webserv_test
CPP				:=	c++
FLAGS			:=	-g3 -Wall -Werror -Wextra -std=c++98
MAKE			:=	make

# ==============================================================================

SRCS_DIR		:=	./srcs

BASE_SRCS :=	$(SRCS_DIR)/ATcpListener.cpp \
							$(SRCS_DIR)/WebServer.cpp \
							$(SRCS_DIR)/config/factory/LocationContext.cpp \
							$(SRCS_DIR)/config/factory/ServerContext.cpp \
							$(SRCS_DIR)/config/factory/AContext.cpp \
							$(SRCS_DIR)/config/factory/ADirective.cpp \
							$(SRCS_DIR)/config/factory/AContextCreator.cpp \
							$(SRCS_DIR)/config/factory/ServerContextCreator.cpp \
							$(SRCS_DIR)/config/factory/LocationContextCreator.cpp \
							$(SRCS_DIR)/config/factory/ADirectiveCreator.cpp \
							$(SRCS_DIR)/config/factory/DirectivesCreator.cpp \
							$(SRCS_DIR)/config/factory/Directives.cpp \
							$(SRCS_DIR)/config/ConfigParser.cpp \
							$(SRCS_DIR)/config/ServerConfig.cpp \
							$(SRCS_DIR)/config/LocationConfig.cpp \
							$(SRCS_DIR)/log/Logger.cpp \
							$(SRCS_DIR)/utils/StringUtils.cpp \
							$(SRCS_DIR)/utils/Conversor.cpp \
							$(SRCS_DIR)/errors/NotAllowedException.cpp \
							$(SRCS_DIR)/errors/NotFoundException.cpp \
							$(SRCS_DIR)/errors/SyntaxErrorException.cpp \
							$(SRCS_DIR)/requests/BaseHTTPRequestHandler.cpp \
							$(SRCS_DIR)/requests/RequestHandler.cpp \
							$(SRCS_DIR)/requests/HTTPStatus.cpp \
							$(SRCS_DIR)/requests/Headers.cpp

SRCS = $(BASE_SRCS) $(SRCS_DIR)/main.cpp

SRCS_TEST = $(BASE_SRCS) $(SRCS_DIR)/tests/main.cpp $(SRCS_DIR)/tests/ConfigParserTest.cpp 

INCLUDES		:= -I./includes/utils  -I./includes/log -I./includes/errors -I./includes/config/factory -I./includes/config -I./includes/requests -I./includes  -I./includes/tests

OBJS			:=	$(SRCS:.cpp=.o)

OBJS_TEST	:=	$(SRCS_TEST:.cpp=.o)

# ==============================================================================
# COLORS
# ==============================================================================

GREEN			:=	\033[1;32m
RED				:=	\033[1;31m
WHT				:=	\033[1;37m
EOC				:=	\033[1;0m

# ==============================================================================
# RECIPES
# ==============================================================================

all:		$(NAME)

%.o:		%.cpp
	@$(CPP) $(FLAGS) $(INCLUDES) -c $< -o $@

$(NAME):			$(OBJS)
					@echo "$(WHT)Compiling $(NAME)...$(EOC)"
					@$(CPP) $(FLAGS) $(OBJS) -o $(NAME)
					@echo "$(GREEN)$(NAME) build completed.$(EOC)"

test: $(NAME_TEST)

$(NAME_TEST): $(OBJS_TEST)
	@echo "$(WHT)Compiling $(NAME_TEST)...$(EOC)"
	@$(CPP) $(FLAGS) $(INCLUDES) $(OBJS_TEST) -o $(NAME_TEST)
	@echo "$(GREEN)$(NAME_TEST) build completed.$(EOC)"

clean:
					@echo "$(WHT)Removing .o files...$(EOC)"
					@rm -f $(OBJS) $(MEMCHECK_LOG_FILE) $(OBJS_TEST)
					@echo "$(GREEN)Clean done.$(EOC)"

fclean:		clean
					@echo "$(WHT)Removing object- and binary -files...$(EOC)"
					@rm -f $(NAME) $(NAME_TEST)
					@echo "$(GREEN)Fclean done.$(EOC)"

re:			fclean all

run:	all
	./$(NAME)

run_test: test
	./$(NAME_TEST)
	
valgrind:
			@valgrind -s --leak-check=full --show-leak-kinds=all \
			--track-origins=yes --log-fd=9 ./$(NAME) webserver2.conf 9>memcheck.log

.PHONY:		all clean fclean re run

exec:
	echo $@