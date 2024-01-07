CC=gcc
CFLAGS=-g -Wall
SRC=src
FILTERS=$(SRC)/util.c $(SRC)/hash_map_oa.c $(SRC)/da.c $(SRC)/node.c
SRCS=$(filter-out $(FILTERS), $(wildcard $(SRC)/*.c))
EXECS=$(patsubst $(SRC)/%.c, $(BINDIR)/%, $(SRCS))
OBJ=obj
DEPS=$(patsubst $(SRC)/%.c, $(OBJ)/%.d, $(SRCS))
BINDIR=bin
OUTPUTDIR=output

all: $(EXECS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

$(EXECS): $(BINDIR)/%: $(OBJ)/%.o $(FILTERS:$(SRC)/%.c=$(OBJ)/%.o)
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -r $(BINDIR)/* $(OBJ)/* $(OUTPUTDIR)/*
