#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
/* #define TOTSEEDS 4 */
#define TOTSEEDS 20

typedef struct {
    long min_dst;
    long min_src;
    long max_src;
    long range_len;
    long diff;
} map;

void swap(long **arr, int idx_a, int idx_b) {
    long temp = (*arr)[idx_a];
    (*arr)[idx_a] = (*arr)[idx_b];
    (*arr)[idx_b] = temp;
}

long extract_number(char **line, char *delim) {
    return strtol(strtok_r(*line, delim, &*line), NULL, 10);
}

void parse_seed_nums(FILE *input_file, long *seeds, char *line) {
    char *seedStr = line;
    for (int i = 0; i < TOTSEEDS; i += 2) {
        seeds[i] = extract_number(&seedStr, "seeds: ");
        seeds[i + 1] = seeds[i] + extract_number(&seedStr, "seeds: ") - 1;
    }
}

void parse_map(char *line, map *m) {
    char *range_str = line;
    char *delim = " ";
    m->min_dst = extract_number(&range_str, delim);
    m->min_src = extract_number(&range_str, delim);
    m->range_len = extract_number(&range_str, delim);
    m->max_src = m->min_src + m->range_len - 1;
    m->diff = m->min_src - m->min_dst;
}

void split_range(long **seeds, int *seeds_size, map m, long a, long b) {
    *seeds_size += 2;
    (*seeds) = realloc(*seeds, sizeof(long) * *seeds_size);
    (*seeds)[*seeds_size - 2] = a - m.diff;
    (*seeds)[*seeds_size - 1] = b - m.diff;
}

void almanac_map(long **seeds, char *line, int *queue_end, int *seeds_size) {
    map m;
    parse_map(line, &m);
    for (int q_cursor = 0; q_cursor < *queue_end; q_cursor += 2) {
        long start = (*seeds)[q_cursor];
        long end = (*seeds)[q_cursor + 1];

        if (start >= m.min_src && start <= m.max_src) {
            if (end <= m.max_src) {
                (*seeds)[q_cursor] -= m.diff;
                (*seeds)[q_cursor + 1] -= m.diff;
                swap(seeds, q_cursor, *queue_end - 1);
                swap(seeds, q_cursor + 1, *queue_end);
                *queue_end -= 2;
                q_cursor = -2;
            } else {
                split_range(seeds, &*seeds_size, m, start, m.max_src);
                (*seeds)[q_cursor] = m.max_src + 1;
                (*seeds)[q_cursor + 1] = end;
            }
        } else if (end >= m.min_src && end <= m.max_src) {
            split_range(seeds, &*seeds_size, m, m.min_src, end);
            (*seeds)[q_cursor] = start;
            (*seeds)[q_cursor + 1] = m.min_src - 1;
        }
    }
}

long get_min_val(long *arr, int arr_size) {
    long min = arr[0];
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

int main(void) {
    /* FILE *input_file = fopen("./input/5-ex", "r"); */
    FILE *input_file = fopen("./input/5", "r");
    if (!input_file) {
        perror("Attempt to open file failed");
        return EXIT_FAILURE;
    }

    char line[MAXLINE];

    long *seeds = calloc(sizeof(long), TOTSEEDS);
    fgets(line, sizeof(line), input_file);
    parse_seed_nums(input_file, seeds, line);

    int seeds_size = TOTSEEDS;
    int queue_end = seeds_size - 1;

    while (fgets(line, sizeof(line), input_file)) {
        if (isalpha(line[0])) {
            queue_end = seeds_size - 1;
        } else if (isdigit(line[0])) {
            almanac_map(&seeds, line, &queue_end, &seeds_size);
        }
    }

    long lowest_loc = get_min_val(seeds, seeds_size);
    printf("\nAoC 2023 Day 5-2\n");
    printf("Lowest Location Number: %lu\n", lowest_loc);

    free(seeds);
    fclose(input_file);
}
