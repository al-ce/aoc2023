#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
#define TOTSEEDS 20

typedef struct {
    long min_dst;
    long min_src;
    long max_src;
    long range_len;
} map;

long extract_number(char **line, char *delim) {
    return strtol(strtok_r(*line, delim, &*line), NULL, 10);
}

void parse_seed_nums(FILE *input_file, long *seeds, char *line) {
    char *seedStr = line;
    for (int i = 0; i < TOTSEEDS; ++i) {
        char *delim = "seeds: ";
        seeds[i] = extract_number(&seedStr, delim);
    }
}

void parse_map(char *line, map *m) {
    char *range_str = line;
    char *delim = " ";
    m->min_dst = extract_number(&range_str, delim);
    m->min_src = extract_number(&range_str, delim);
    m->range_len = extract_number(&range_str, delim);
    m->max_src = m->min_src + m->range_len - 1;
}

// Update the seeds array according to the current map
// Return the new queue_end if any values were updated
int almanac_map(long *seeds, char *line, int queue_end) {
    map m;
    parse_map(line, &m);
    for (int q_cursor = 0; q_cursor <= queue_end; ++q_cursor) {
        long curr = seeds[q_cursor];
        if (curr >= m.min_src && curr <= m.max_src) {
            seeds[q_cursor] -= m.min_src - m.min_dst;
            curr = seeds[q_cursor];
            seeds[q_cursor] = seeds[queue_end];
            seeds[queue_end] = curr;
            --queue_end;
            q_cursor = -1;
        }
    }
    return queue_end;
}

int main(void) {
    FILE *input_file = fopen("./input/5", "r");
    if (!input_file) {
        perror("Attempt to open file failed");
        return EXIT_FAILURE;
    }

    char line[MAXLINE];

    long seeds[TOTSEEDS];
    fgets(line, sizeof(line), input_file);
    parse_seed_nums(input_file, seeds, line);

    int queue_end = TOTSEEDS - 1;
    while (fgets(line, sizeof(line), input_file)) {
        if (isdigit(line[0])) {
            queue_end = almanac_map(seeds, line, queue_end);
        } else if (line[0] == '\n') {
            queue_end = TOTSEEDS - 1;
        }
    }

    long lowest_loc = seeds[0];
    for (int i = 1; i < TOTSEEDS; ++i) {
        if (seeds[i] < lowest_loc) {
            lowest_loc = seeds[i];
        }
    }

    printf("Lowest Location Number: %lu\n", lowest_loc);

    fclose(input_file);
}
