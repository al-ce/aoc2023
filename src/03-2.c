
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

typedef struct gear {
    int col;
    int first;
    int second;
    int gear_ratio;
    int found;
} gear;

void update_gear(gear *curr_gear, int val) {
    curr_gear->first = !curr_gear->first ? val : 0;
    curr_gear->second = curr_gear->first ? val : 0;
    ++curr_gear->found;
    curr_gear->gear_ratio *= val;
}

int move_cursor(char *lines[], int row, int cursor, gear *s) {
    int initial_position = cursor;
    char *line = lines[row];
    int val = 0;
    if (initial_position > s->col + 1 || row < 0 || row == strlen(line) ||
        cursor < 0 || cursor >= strlen(line) || !isdigit(line[cursor])) {
        return initial_position;
    }

    // Move cursor to leftmost digit
    while (cursor >= 0 && isdigit(line[cursor])) {
        --cursor;
    }

    // Update value until end of number
    while (isdigit(line[++cursor])) {
        int digit = line[cursor] - '0';
        val = val * 10 + digit;
    }

    // Update the gear if the number is in a valid position
    if (val > 0) {
        update_gear(s, val);
        return cursor;
    }
    return initial_position;
}

int check_gear(char **lines, int row, int col) {
    gear g = {col, 0, 0, 1, 0};

    // Check curr + adjacent rows for a number
    for (int i = row - 1; i <= row + 1; ++i) {
        // Check curr + adjacent cols for a number
        for (int cursor = col - 1; cursor <= col + 1; ++cursor) {
            cursor = move_cursor(lines, i, cursor, &g);
        }
    }

    // Only return a gear ratio if exactly two numbers were found
    int gear_ratio = g.found == 2 ? g.gear_ratio : 0;
    return gear_ratio;
}

int main(void) {

    char **lines = file_to_str_array("./input/3");
    int sum_of_gear_ratios = 0;

    for (int row = 0; lines[row]; ++row) {
        for (int col = 0; col < strlen(lines[row]); ++col) {
            char c = lines[row][col];
            if (c == '*') {
                sum_of_gear_ratios += check_gear(lines, row, col);
            }
        }
    }

    printf("Sum of gear ratios: %d\n\n", sum_of_gear_ratios);

    for (int row = 0; lines[row]; ++row) {
        free(lines[row]);
    }
    free(lines);
}
