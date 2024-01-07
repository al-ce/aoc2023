CC=gcc
CFLAGS=-g -Wall
SRC=src
FILTERS=$(SRC)/util.c $(SRC)/hash_map_oa.c
SRCS=$(filter-out $(FILTERS), $(wildcard $(SRC)/*.c))
EXECS=$(patsubst $(SRC)/%.c, $(BINDIR)/%, $(SRCS))
OBJ=obj
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BINDIR=bin
OUTPUTDIR=output

all: $(EXECS)

$(EXECS): $(BINDIR)/%: $(OBJ)/%.o $(OBJ)/util.o $(OBJ)/hash_map_oa.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r $(BINDIR)/* $(OBJ)/* $(OUTPUTDIR)/*
