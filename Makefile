# ==============================================================================
# VARIABLES
# ==============================================================================
NAME			:=	webserver
CPP				:=	c++
FLAGS			:=	-g3 -Wall -Werror -Wextra -std=c++98
MAKE			:=	make

# ==============================================================================

SRCS_DIR		:=	./srcs
SRCS			:=	$(SRCS_DIR)/ATcpListener.cpp \
							$(SRCS_DIR)/WebServer.cpp \
							$(SRCS_DIR)/config/factory/AContext.cpp \
							$(SRCS_DIR)/config/factory/LocationContext.cpp \
							$(SRCS_DIR)/config/factory/ServerContext.cpp \
							$(SRCS_DIR)/config/factory/ADirective.cpp \
							$(SRCS_DIR)/config/factory/AContextCreator.cpp \
							$(SRCS_DIR)/config/factory/ServerContextCreator.cpp \
							$(SRCS_DIR)/config/factory/LocationContextCreator.cpp \
							$(SRCS_DIR)/config/factory/ADirectiveCreator.cpp \
							$(SRCS_DIR)/config/factory/DirectivesCreator.cpp \
							$(SRCS_DIR)/config/factory/Directives.cpp \
							$(SRCS_DIR)/utils/StringUtils.cpp \
							$(SRCS_DIR)/config/ConfigParser.cpp \
							$(SRCS_DIR)/log/Logger.cpp \
							$(SRCS_DIR)/main.cpp

INCLUDES		:= -I./includes/utils  -I./includes/log -I./includes/config/factory -I./includes/config -I./includes 

OBJS			:=	$(SRCS:.cpp=.o)

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
					@echo -e "$(WHT)Compiling $(NAME)...$(EOC)"
					@$(CPP) $(FLAGS) $(OBJS) -o $(NAME)
					@echo -e "$(GREEN)$(NAME) build completed.$(EOC)"

clean:
					@echo -e "$(WHT)Removing .o files...$(EOC)"
					@rm -f $(OBJS) $(RESULT_LOG_FILE) $(INPUT_LOG_FILE)
					@echo -e "$(GREEN)Clean done.$(EOC)"

fclean:		clean
					@echo -e "$(WHT)Removing object- and binary -files...$(EOC)"
					@rm -f $(NAME)
					@echo -e "$(GREEN)Fclean done.$(EOC)"

re:			fclean all

run:	all
	./$(NAME)
	
valgrind:
			@valgrind -s --leak-check=full --show-leak-kinds=all \
			--track-origins=yes --log-fd=9 ./$(NAME) 9>memcheck.log

.PHONY:		all clean fclean re run
