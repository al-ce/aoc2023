#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

#define LINE_MAX 256
#define delimiters " ,;\n"

typedef struct color_min color_min;
struct color_min {
    char *color;
    int min;
};


// Update the minimum value of a color if `num` is greater than current min
void update_min(char *buf, color_min color_mins[], int num) {
    for (int j = 0; j < 3; ++j) {
        color_min *curr = &color_mins[j];
        if (!strcmp(curr->color, buf)  && num > curr->min) {
                curr->min = num;
        }
    }
}

// Set the minimum value needed for each color for a given game
void set_color_mins(char *line, int col_idx, color_min *color_mins) {
    char buf[LINE_MAX];
    int num = 0;
    int b = 0;
    for (int i = col_idx + 2; line[i] != '\0'; ++i) {
        buf[b] = line[i];
        if (find_char(delimiters, line[i]) == -1) {
            ++b;
        } else if (isdigit(line[i - 1])) {
            buf[b] = '\0';
            num = str_to_int(buf);
            b = 0;
        } else if (isalpha(line[i - 1])) {
            buf[b] = '\0';
            update_min(buf, color_mins, num);
            b = 0;
        }
    }
}

// Calculate the power of the minimum set of cubes needed for the game
int power_of_mins(char *line, int col_idx) {

    color_min color_mins[] = {
        {.color = "red", .min = 0},
        {.color = "blue", .min = 0},
        {.color = "green", .min = 0},
    };

    set_color_mins(line, col_idx, color_mins);

    int power = 1;
    for (int i = 0; i < 3; ++i) {
        power *= color_mins[i].min;
    }
    return power;
}

// Return the sum of the power of the minimum set of cubes for each game
int cubes2(FILE *input) {
    char c;
    char line[LINE_MAX];
    int line_idx = 0;
    int sum = 0;

    while (fread(&c, sizeof(char), 1, input)) {
        line[line_idx] = c;
        if (c == '\n') {
            line[line_idx + 1] = '\0';
            int col_idx = find_char(line, ':');
            sum += power_of_mins(line, col_idx);
            line_idx = 0;
        } else {
            ++line_idx;
        }
    }
    return sum;
};

int main(void) {
    FILE *input = fopen("./input/2", "r");
    int sum = cubes2(input);
    printf("\n%d\n", sum);
    fclose(input);
}
