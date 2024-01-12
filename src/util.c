#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define assert__(x) for (; !(x); assert(x))

// Read a file into an array of strings. Caller is responsible for freeing
// the array and its contents.
char **file_to_str_array(char *filename) {

    FILE *input_file = fopen(filename, "r");
    fopen_success_check(input_file);

    int lines_size = 1, str_size = 1;
    char *line = calloc(str_size, sizeof(char *));
    char **lines = calloc(lines_size, sizeof(char *));
    int lines_read = 0, chars_read = 0;

    char c;
    while (fread(&c, sizeof(char), 1, input_file)) {
        ++chars_read;
        if (chars_read == str_size) {
            line = realloc(line, str_size * 2 * sizeof(char));
            str_size *= 2;
        }

        line[chars_read - 1] = c;

        if (c == '\n') {
            ++lines_read;
            if (lines_read == lines_size) {
                lines = realloc(lines, lines_size * 2 * sizeof(char *));
                lines_size *= 2;
            }

            line[chars_read + 1] = '\0';
            lines[lines_read - 1] = line;
            chars_read = 0;
            str_size = 1;
            line = calloc(str_size, sizeof(char *));
        }
    }

    free(line); // Free the last allocation, which was not used.
    fclose(input_file);
    return lines;
}

int count_lines_in_file(char *input) {
    FILE *input_file = fopen(input, "r");
    fopen_success_check(input_file);
    int lines = 0;
    char c;
    while (fread(&c, sizeof(char), 1, input_file)) {
        if (c == '\n') {
            ++lines;
        }
    }
    fclose(input_file);
    return lines;
}

// Return the index of a char in a string, or -1 if not found.
int find_char(char *line, char target) {
    char c = '\0';
    int k = 0;
    char canditate = line[k];
    while (c != target && canditate != '\0') {
        c = canditate;
        canditate = line[++k];
    }
    return c == target ? k - 1 : -1;
}

int fopen_success_check(FILE *file) {
    if (!file) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    return 0;
}

// Get the length of a string representation of an integer
int get_strlen_of_int(int num) {
    int num_str_len = 0;
    while (num) {
        ++num_str_len;
        num = num / 10;
    }
    return num_str_len;
}

// Convert an integer to a string representation (assumes unsigned)
int int_to_str(int sum, char *sum_str, int sum_strlen) {
    for (int i = 0; i < sum_strlen; i++) {
        sum_str[sum_strlen - i - 1] = (sum % 10) + 48;
        sum = sum / 10;
    }
    return *sum_str;
}

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

// Convert a string to an integer (assumes unsigned)
int str_to_int(char *str) {
    int str_as_int = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        int curr = str[i] - '0';
        assert__(curr <= 9 && curr >= 0) {
            printf("\nERROR: %s is not a valid integer\n\n", str);
        }
        str_as_int = str_as_int * 10 + curr;
    }
    return str_as_int;
}

int strstarts(char *str, char *startswith) {
    int len_starts = strlen(startswith);
    int len_str = strlen(str);
    if (len_starts > len_str) {
        return 0;
    }
    for (int i = 0; i < len_starts; ++i) {
        if (!(str[i] == startswith[i])) {
            return 0;
        }
    }
    return 1;
}
