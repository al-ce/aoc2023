#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define MAX_POWER 3

int is_symbol(char c) { return c != '.' && ispunct(c); }

int find_symbol(char *line, int start, int end) {
    int start_adj = max(0, start - 1);
    int end_adj = min(end, strlen(line) + 1);
    for (int i = start_adj; i <= end_adj; ++i) {
        if (is_symbol(line[i])) {
            return 1;
        }
    }
    return 0;
}

char *found_part_num(char *num, int cursor, char **lines, int row) {

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
    int prev = (start > 0 && is_symbol(lines[row][start - 1]));
    int next = (cursor < line_len - 1 && is_symbol(lines[row][cursor]));
    int above = (row > 0 && find_symbol(lines[row - 1], start, cursor));
    int below = (lines[row + 1] && find_symbol(lines[row + 1], start, cursor));
    if (prev || next || above || below) {
        return num;
    }

    // If no symbol is adjacent to the number, reset `num` to an empty string,
    // indicating that it is not a valid part number
    free(num);
    num = calloc(1, sizeof(char));
    return num;
}

int get_part_nums_in_line(char **lines, int row) {

    int cursor = 0;
    int to_add = 0;

    while (cursor < strlen(lines[row])) {
        char *num = calloc(1, sizeof(char));
        num = found_part_num(num, cursor, lines, row);
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
    int sum = 0;

    for (int i = 0; lines[i]; ++i) {
        sum += get_part_nums_in_line(lines, i);
    }

    printf("%d\n", sum);
    /* printf("hello"); */

    for (int i = 0; lines[i]; ++i) {
        free(lines[i]);
    }
    free(lines);
}
