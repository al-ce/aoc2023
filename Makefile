CC=gcc
CFLAGS=-g -Wall

SRC=src
SRCS=$(filter-out $(SRC)/util.c, $(wildcard $(SRC)/*.c))
EXECS=$(patsubst $(SRC)/%.c, $(BINDIR)/%, $(SRCS))

OBJ=obj
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
OBJS += $(OBJ)/util.o

BINDIR=bin
OUTPUTDIR=output

all: $(EXECS)

$(EXECS): $(BINDIR)/%: $(OBJ)/%.o $(OBJ)/util.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/util.o: $(SRC)/util.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r $(BINDIR)/* $(OBJ)/* $(OUTPUTDIR)/*
