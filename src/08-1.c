#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map_oa.h"

#define MAX_SIZE 512
#define KV_SIZE 17

int main(void) {
    char instructions[MAX_SIZE];
    char kv_line[KV_SIZE];
    char key[4] = {0};
    char value[9] = {0};

    FILE *fp = fopen("./input/8", "r");
    if (fp == NULL) {
        perror("Attempt to open file failed\n");
        return EXIT_FAILURE;
    }

    fgets(instructions, MAX_SIZE, fp);
    fseek(fp, 1, SEEK_CUR); // consume newline

    HashMap *hm = hm__init__(7, 2);
    while (fgets(kv_line, KV_SIZE, fp)) {
        strncpy(key, kv_line, 3);
        strncpy(value, kv_line + 7, 8);
        hm_put(hm, key, value);
    }

    int i = 0;
    int steps = 0;
    int num_instructs = strlen(instructions);

    strncpy(key, "AAA", 4);
    while (strcmp(key, "ZZZ")) {
        char direction = instructions[i];
        char *curr_value = hm_get(hm, key);
        int start_offset = direction == 'L' ? 0 : 5;
        strncpy(key, curr_value + start_offset, 3);
        ++steps;
        ++i;
        i = i == num_instructs - 1 ? 0 : i;
    }

    printf("\nAoC 2023 Day 8 Part 1\n");
    printf("\nSteps: %d\n", steps);

    hm_dealloc(hm);
    fclose(fp);
}
