#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define MAX_POWER 3

typedef struct gear gear;
struct gear {
    int row;
    int col;
    int first;
    int second;
};

typedef struct node node;
struct node {
    gear val;
    node *next;
};

int is_gear(char c) { return c = '*'; }

int find_gear(char *line, int start, int end) {
    start = max(0, start - 1);
    end = min(end, strlen(line) + 1);
    for (int i = start; i <= end; ++i) {
        if (is_gear(line[i])) {
            return start + i;
        }
    }
    return -1;
}

int find_above(char **lines, int row, int start, int end) {
    return row == 0 ? -1 : find_gear(lines[row - 1], start, end);
}

int find_below(char **lines, int row, int start, int end) {
    return !lines[row + 1] ? -1 : find_gear(lines[row + 1], start, end);
}

int check_left(char *line, int start) {
    return start > 0 && is_gear(line[start - 1]);
}

int check_right(char *line, int cursor) {
    return cursor < strlen(line) - 1 && is_gear(line[cursor]);
}

gear *find_gear_ratio(char *num, int cursor, char **lines, int row) {

    int start = cursor;
    int line_len = strlen(lines[row]);

    while (cursor < line_len && isdigit(lines[row][cursor])) {
        if (cursor - start == sizeof(num)) {
            num = realloc(num, sizeof(num) * 2);
        }
        num[cursor - start] = lines[row][cursor];
        ++cursor;
    }



    // If we didn't build a number, early return
    if (!strlen(num)) {
        return num;
    }

    // Check for part-number indicator symbol
    int prev = (start > 0 && is_gear(lines[row][start - 1]));
    int next = (cursor < line_len - 1 && is_gear(lines[row][cursor]));
    int above = (row > 0 && find_gear(lines[row - 1], start, cursor));
    int below = (lines[row + 1] && find_gear(lines[row + 1], start, cursor));
    if (prev || next || above || below) {
        return num;
    }

    // If no symbol is adjacent to the number, reset `num` to an empty string,
    // indicating that it is not a valid part number
    free(num);
    num = calloc(1, sizeof(char));
    return num;
}

int find_gears_in_line(char **lines, int row) {

    int cursor = 0;
    int to_add = 0;

    while (cursor < strlen(lines[row])) {
        char *num = calloc(1, sizeof(char));
        num = find_gear_ratio(num, cursor, lines, row);
        if (strlen(num)) {
            cursor += strlen(num);
            to_add += str_to_int(num);
        } else {
            ++cursor;
        }
        free(num);
    }

    return to_add;
}

int main(void) {

    char **lines = file_to_str_array("./input/3");
    int sum_of_ratios = 0;
    for (int i = 0; lines[i]; ++i) {
        sum_of_ratios += find_gears_in_line(lines, i);
    }

    printf("%d\n", sum_of_ratios);

    for (int i = 0; lines[i]; ++i) {
        free(lines[i]);
    }
    free(lines);
}
