#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

#define LINE_MAX 256
#define RED_MAX 12
#define GREEN_MAX 13
#define BLUE_MAX 14
#define delimiters " ,;\n"

// Extract the game id from a line
int get_game_id(char *line, int col_idx) {
    char game_id_str[3];
    for (int i = 5, k = 0; i < col_idx; ++i, ++k) {
        game_id_str[k] = line[i];
    }
    int game_id = str_to_int(game_id_str);
    return game_id;
}

// Return the max value of a color
int get_color_max(char *color) {
    if (!strcmp(color, "red")) {
        return RED_MAX;
    } else if (!strcmp(color, "green")) {
        return GREEN_MAX;
    } else if (!strcmp(color, "blue")) {
        return BLUE_MAX;
    }
    return 0;
}

// Determine if a game with the given draws is possible
int is_possible(char *line, int col_idx) {
    char buf[LINE_MAX];
    int num = 0;
    int b = 0;

    for (int i = col_idx + 2; line[i] != '\0'; ++i) {
        buf[b] = line[i];
        if (find_char(delimiters, line[i]) == -1) {
            ++b;
        } else {
            buf[b] = '\0';
            if (isdigit(line[i - 1])) {
                num = str_to_int(buf);
            } else if (isalpha(line[i - 1]) && num > get_color_max(buf)) {
                return 0;
            }
            b = 0;
        }
    }
    return 1;
}

// Return the sum of all possible game ids
int cubes1(FILE *input) {
    char c;
    char line[LINE_MAX];
    int line_idx = 0;
    int sum = 0;

    while (fread(&c, sizeof(char), 1, input)) {
        line[line_idx] = c;
        if (c == '\n') {
            line[line_idx + 1] = '\0';
            int col_idx = find_char(line, ':');
            int game_id = get_game_id(line, col_idx);
            sum += game_id * is_possible(line, col_idx);
            line_idx = 0;
        } else {
            ++line_idx;
        }
    }
    return sum;
};

int main(void) {
    FILE *input = fopen("./input/2", "r");
    int sum = cubes1(input);
    printf("\n%d\n", sum);
    fclose(input);
}
