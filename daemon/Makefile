
SRC_NAME = main.c \
						server.c \
						localclient.c \
						p2p.c \
						p2p_host.c \
						command.c

OBJ_NAME = $(SRC_NAME:.c=.o)

NAME = cryptstored

SRC_PATH = ./sources/
INC_PATH = ./includes/
INC_GLOB_PATH = ../includes/
OBJ_PATH = ./objects/
BIN_PATH = ../bin/

BINARY = $(BIN_PATH)$(NAME)

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
INC_GLOB = $(addprefix -I,$(INC_GLOB_PATH))



all: $(BINARY)

$(BINARY): $(OBJ)
	@mkdir -p $(BIN_PATH)
	$(CC) $(CFLAGS) $(INC) $(INC_GLOB) -o $(BINARY) $(OBJ)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INC) $(INC_GLOB) -o $@ -c $<

clean:
	rm -rf $(OBJ_PATH)

mrproper: clean
	rm -rf $(BINARY)

re: mrproper all

install: all
	cp $(BINARY) /usr/sbin/
	cp $(NAME).service /etc/systemd/system/

uninstall:
	systemctl stop $(NAME)
	rm /usr/sbin/$(NAME)
	rm /etc/systemd/system/$(NAME).service
