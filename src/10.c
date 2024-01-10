#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Coord {
    int row;
    int col;
} Coord;

typedef struct Loop {
    Coord curr;
    Coord prev;
    char curr_type;
    Coord *vertices;
    int length;
    int capacity;
} Loop;

void append_vertex(Loop *loop) {
    ++loop->length;
    if (loop->length == loop->capacity) {
        loop->capacity *= 2;
        int new_size = sizeof(Coord) * loop->capacity;
        loop->vertices = realloc(loop->vertices, new_size);
    }
    loop->vertices[loop->length - 1] = loop->curr;
}

Coord find_start(char **diagram) {
    Coord start = {-1, -1};
    for (int i = 0; diagram[i]; ++i) {
        char *s = strchr(diagram[i], 'S');
        if (s) {
            start.row = i;
            start.col = s - diagram[i];
            break;
        }
    }
    return start;
}

void look(Loop *loop, Coord dir, char *connects, char **diagram) {
    Coord next = {loop->curr.row + dir.row, loop->curr.col + dir.col};
    Coord prev = loop->prev;

    char *can_move = strchr(connects, loop->curr_type);
    int backtracking = prev.row == next.row && prev.col == next.col;

    if (can_move && !backtracking) {
        loop->prev = loop->curr;
        loop->curr = next;
        loop->curr_type = diagram[next.row][next.col];
        append_vertex(loop);
    }
}

void traverse(Loop *loop, char **diagram) {
    char *directions[] = {"SJL|", "SF7|", "SFL-", "S7J-"};
    Coord coords[] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

    char curr_type = '\0';
    while (curr_type != 'S') {
        for (int i = 0; i < 4; ++i) {
            look(loop, coords[i], directions[i], diagram);
        }
        curr_type = loop->curr_type;
    }
}

int get_interior_tiles(Loop *loop) {
    int sum = 0;
    for (int i = 0; i < loop->length; ++i) {
        int prev_idx = i - 1 < 0 ? loop->length - 1 : i - 1;
        Coord p_0 = loop->vertices[i];
        Coord p_1 = loop->vertices[prev_idx];
        sum += (p_1.col * p_0.row) - (p_0.col * p_1.row);
    }
    return (sum - loop->length) / 2 + 1;
}

int main(void) {

    char **diagram = file_to_str_array("./input/10");
    Coord start = find_start(diagram);

    Loop loop = {.curr = start, .curr_type = 'S', .length = 0, .capacity = 7};
    loop.vertices = calloc(sizeof(Coord), loop.capacity);
    loop.vertices[loop.length] = loop.curr;

    traverse(&loop, diagram);
    int farthest = loop.length / 2 + loop.length % 2;
    int interior = get_interior_tiles(&loop);

    printf("\nAoC 2023 Day 10\n");
    printf("Loop starts at: %d %d\n", start.row, start.col);
    printf("Loop length: %d\n", loop.length);
    printf("Part 1: Farthest pipe is %d pipes away\n", farthest);
    printf("Part 2: There are %d interior tiles\n", interior);
}
