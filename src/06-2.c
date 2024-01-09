#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPLITS 4
#define STRMAX 256

long fix_kerning(FILE *input_file, char *str, char *line, char *delim) {
    for (int i = 0; i < SPLITS; ++i) {
        char *part = strtok_r(line, delim, &line);
        strcat(str, part);
    }
    return strtol(str, NULL, 10);
}

void quadratic_formula(long double arr[], long a, long b, long c) {
    long double radicand = (b * b) - (4 * a * c);
    arr[0] = (-b + sqrt(radicand)) / 2;
    arr[1] = (-b - sqrt(radicand)) / 2;
}

int get_range(long double high, long double low) {
    return (int)floor(high + 1) - (int)ceil(low);
}

int main(void) {

    FILE *input_file = fopen("./input/6", "r");
    if (!input_file) {
        perror("Attempt to open file failed");
        return EXIT_FAILURE;
    }

    char line[256];
    char *time_str = calloc(STRMAX, sizeof(char));
    char *record_str = calloc(STRMAX, sizeof(char));

    fgets(line, sizeof(line), input_file);
    long time = fix_kerning(input_file, time_str, line, "Time: ");
    fgets(line, sizeof(line), input_file);
    long record = fix_kerning(input_file, record_str, line, "Distance: ");

    long double factors[2];
    quadratic_formula(factors, -1, time, -record + 1);

    int ways = get_range(factors[0], factors[1]);
    printf("\nAoC 2023 - Day 6-2\n");
    printf("\n%d ways to win\n", ways);

    free(time_str);
    free(record_str);
    fclose(input_file);
}
