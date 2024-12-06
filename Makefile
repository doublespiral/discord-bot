# Makefile #
CC ?= gcc

DIR_SRC = source
DIR_BIN = bin

C_FLAGS  = -O0 -g -pthread -Wall
LD_LIBS  = -ldiscord -lcurl


link: compile
	$(CC) $(DIR_BIN)/main.o \
		$(DIR_BIN)/command.o \
		$(DIR_BIN)/message.o \
		$(DIR_BIN)/rules.o \
		$(C_FLAGS) $(LD_LIBS) -o bot/bot


compile:
	$(CC) -c $(DIR_SRC)/commands/message.c $(C_FLAGS) -o $(DIR_BIN)/message.o
	$(CC) -c $(DIR_SRC)/commands/rules.c $(C_FLAGS) -o $(DIR_BIN)/rules.o
	$(CC) -c $(DIR_SRC)/commands/command.c $(C_FLAGS) -o $(DIR_BIN)/command.o

	$(CC) -c $(DIR_SRC)/main.c $(C_FLAGS) -o $(DIR_BIN)/main.o


