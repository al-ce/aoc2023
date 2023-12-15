#include <ctype.h>
#include <stdio.h>

#define DIGITS 9
#define LINE_MAX 256

char *num_names[] = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
};

int lookahead_match(char *line, int idx) {
    for (int j = 0; j < DIGITS; ++j) {
        char *num = num_names[j];
        int k = 0;
        while (line[idx + k] == num[k]) {
            ++k;
            if (num[k] == '\0') {
                return j + 1;
            }
        }
    }
    return 0;
}

int sum_of_outer_ints(char *line, int line_len) {
    int first = 0, last = 0;
    for (int i = 0; i < line_len; ++i) {
        if (isdigit(line[i])) {
            last = line[i] - '0';
        } else {
            int match = lookahead_match(line, i);
            last = match ? match : last;
        }
        first = first ? first : last;
    }
   return first * 10 + last;
}

int trebuchet2(FILE *input) {
    char c;
    char line[LINE_MAX];
    int line_idx = 0;
    int sum = 0;

    while (fread(&c, sizeof(char), 1, input)) {
        line[line_idx] = c;
        if (c == '\n') {
            sum += sum_of_outer_ints(line, line_idx);
            line_idx = 0;
        } else {
            ++line_idx;
        }
    }
    return sum;
};

int main(void) {
    FILE *input = fopen("./input/1", "r");
    int sum = trebuchet2(input);
    printf("\n%d\n", sum);
    fclose(input);
}
