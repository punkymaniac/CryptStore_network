
SRC_NAME = main_test.c \
						server.c

OBJ_NAME = $(SRC_NAME:.c=.o)

NAME = a.out

SRC_PATH = ./sources/
INC_PATH = ./includes/
OBJ_PATH = ./objects/

CC = gcc
CFLAGS = -Wall -Werror -Wextra

ifeq ($(DEBUG), true)
	CFLAGS += -g
endif
ifeq ($(PROF), true)
	CFLAGS += -pg
endif

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))
INC = $(addprefix -I,$(INC_PATH))



all: $(NAME)

help:
	@echo "Option variable:"
	@echo " DEBUG=true\t: debug compilation"
	@echo " PROF=true\t: profiling compilation"

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(OBJ_PATH)

mrproper: clean
	rm -rf $(NAME)

re: mrproper all
