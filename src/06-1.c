#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RACES 4

void parse_nums(FILE *input_file, int *arr, char *line, char *delim) {
    for (int i = 0; i < RACES; ++i) {
        arr[i] = (int)strtol(strtok_r(line, delim, &line), NULL, 10);
    }
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
    int times[RACES];
    int records[RACES];

    fgets(line, sizeof(line), input_file);
    parse_nums(input_file, times, line, "Time: ");
    fgets(line, sizeof(line), input_file);
    parse_nums(input_file, records, line, "Distance: ");

    int product = 1;
    for (int i = 0; i < RACES; ++i) {
        long double factors[2];
        quadratic_formula(factors, -1, times[i], -records[i] + 1);
        product *= get_range(factors[0], factors[1]);
    }

    printf("\nAoC 2023 - Day 6-1\n");
    printf("\n%d ways to win\n", product);

    fclose(input_file);
}
