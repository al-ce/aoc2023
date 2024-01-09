#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256
#define INIT_LEN 21

long extrapolate_next_num(long *arr, int len, int direction) {
    if (len == 0) {
        return 0;
    }
    long diffs[len - 1];
    for (int i = 0; i < len - 1; ++i) {
        diffs[i] = arr[i + 1] - arr[i];
    }
    if (direction == 1) {
        return arr[len - 1] + extrapolate_next_num(diffs, len - 1, direction);
    }
    return arr[0] - extrapolate_next_num(diffs, len - 1, direction);

}

void parse_nums_line(long *arr, char *line) {
    for (int i = 0; i < INIT_LEN; ++i) {
        char *delim = " ";
        arr[i] = strtoll(strtok_r(line, delim, &line), NULL, 10);
    }
}

int main(void) {
    FILE *fp = fopen("./input/9", "r");
    char line[MAX];
    long oasis_vals[INIT_LEN];
    long prev_sum = 0;
    long next_sum = 0;

    while (fgets(line, sizeof(line), fp)) {
        parse_nums_line(oasis_vals, line);
        long prev = extrapolate_next_num(oasis_vals, INIT_LEN, 0);
        prev_sum += prev;
        long next = extrapolate_next_num(oasis_vals, INIT_LEN, 1);
        next_sum += next;
    }

    printf("\nAoC 2023 - Day 9 Part 1\n");
    printf("Sum of extrapolated vals:\n");
    printf("Previous: %ld\nNext:     %ld\n", prev_sum, next_sum);
}
