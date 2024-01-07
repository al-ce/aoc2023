#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map_oa.h"

#define MAX_SIZE 512
#define KV_SIZE 17

int main(void) {

    FILE *fp = fopen("./input/8", "r");
    if (fp == NULL) {
        perror("Attempt to open file failed\n");
        return EXIT_FAILURE;
    }

    char instructions[MAX_SIZE];
    fgets(instructions, MAX_SIZE, fp);
    fseek(fp, 1, SEEK_CUR); // consume newline

    HashMap *hm = hm__init__(7, 2);
    char kv_line[KV_SIZE];
    char key[4] = {0};
    char value[9] = {0};
    while (fgets(kv_line, KV_SIZE, fp)) {
        strncpy(key, kv_line, 3);
        strncpy(value, kv_line + 7, 8);
        hm_put(hm, key, value);
    }

    int i = 0;
    int steps = 0;
    int instruct_size = strlen(instructions);
    char curr_key[4] = {0};
    strncpy(curr_key, "AAA", 4);
    while (strcmp(curr_key, "ZZZ")) {
        char instruction = instructions[i];
        char *curr_value = hm_get(hm, curr_key);
        if (instruction == 'R') {
            strncpy(curr_key, curr_value + 5, 3);
        } else if (instruction == 'L') {
            strncpy(curr_key, curr_value, 3);
        }
        ++steps;
        ++i;
        if (i == instruct_size - 1) {
            i = 0;
        }
    }

    printf("\nAoC 2023 Day 8 Part 1: %s\n", curr_key);
    printf("\nSteps: %d\n", steps);

    hm_dealloc(hm);
    fclose(fp);
}
