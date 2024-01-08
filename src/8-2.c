#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "da.h"
#include "hash_map_oa.h"

#define MAX_SIZE 512
#define KV_SIZE 17

long euclidean_gcd(long a, long b) {
    if (b == 0) {
        return a;
    }
    return euclidean_gcd(b, a % b);
}

long get_lcm_of_array(long *arr, int arr_size) {
    for (int i = 1; i < arr_size; ++i) {
        long a = arr[i-1];
        long b = arr[i];
        arr[i] = a / euclidean_gcd(a, b) * b;
    }
    return arr[arr_size-1];
}

int main(void) {

    FILE *fp = fopen("./input/8", "r");
    /* FILE *fp = fopen("./input/8-ex2", "r"); */
    if (fp == NULL) {
        perror("Attempt to open file failed\n");
        return EXIT_FAILURE;
    }

    char instructions[MAX_SIZE];
    fgets(instructions, MAX_SIZE, fp);
    fseek(fp, 1, SEEK_CUR); // consume newline

    DynamicArray *da = da__init__(0);
    HashMap *hm = hm__init__(7, 2);
    char kv_line[KV_SIZE];
    char key[4] = {0};
    char value[9] = {0};
    while (fgets(kv_line, KV_SIZE, fp)) {
        strncpy(key, kv_line, 3);
        strncpy(value, kv_line + 7, 8);
        if (key[2] == 'A') {
            node *n = make_node(key, value);
            da_append(da, n);
            node_dealloc(n);
        }
        hm_put(hm, key, value);
    }

    int size = da->size;
    long *steps_arr = calloc(sizeof(long), size);

    for (int k = 0; k < size; ++k) {
        int i = 0;
        long steps = 0;
        int instruct_size = strlen(instructions);
        node *n = da_get(da, k);
        char curr_key[4] = {0};
        char *curr_value;
        strncpy(curr_key, n->key, 4);
        while (curr_key[2] != 'Z') {
            char instruction = instructions[i];
            curr_value = hm_get(hm, curr_key);
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
        steps_arr[k] = steps;
    }

    long lcm = get_lcm_of_array(steps_arr, size);
    printf("\nAoC 2023 Day 8 Part 2:\n");
    printf("LCM: %ld\n", lcm);

    free(steps_arr);
    da_dealloc(da);
    hm_dealloc(hm);
    fclose(fp);
}
