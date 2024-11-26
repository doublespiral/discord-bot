# Makefile #
CC ?= gcc

DIR_SRC = source
DIR_BIN = bot

C_FLAGS  = -O0 -g -pthread -Wall
LD_LIBS  = -ldiscord -lcurl


build:
	$(CC) $(DIR_SRC)/main.c $(C_FLAGS) $(LD_LIBS) -o $(DIR_BIN)/main