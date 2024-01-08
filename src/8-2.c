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
        long a = arr[i - 1];
        long b = arr[i];
        arr[i] = a / euclidean_gcd(a, b) * b;
    }
    return arr[arr_size - 1];
}

long calculate_steps(HashMap *hm, char *instructions, char *key) {
    int i = 0;
    long steps = 0;
    int num_instructs = strlen(instructions);

    while (key[2] != 'Z') {
        char *curr_value = hm_get(hm, key);
        int start_offset = instructions[i] == 'L' ? 0 : 5;
        strncpy(key, curr_value + start_offset, 3);
        ++steps;
        ++i;
        i = i == num_instructs - 1 ? 0 : i;
    }
    return steps;
}

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

    DynamicArray *da = da__init__(0);
    HashMap *hm = hm__init__(7, 2);
    while (fgets(kv_line, KV_SIZE, fp)) {
        strncpy(key, kv_line, 3);
        strncpy(value, kv_line + 7, 8);
        if (key[2] == 'A') {
            node n = {key, value};
            da_append(da, &n);
        }
        hm_put(hm, key, value);
    }

    int size = da->size;
    long steps_arr[size];
    for (int k = 0; k < size; ++k) {
        node *n = da_get(da, k);
        strncpy(key, n->key, 4);
        steps_arr[k] = calculate_steps(hm, instructions, key);
    }

    long lcm = get_lcm_of_array(steps_arr, size);

    printf("\nAoC 2023 Day 8 Part 2:\n");
    printf("LCM: %ld\n", lcm);

    da_dealloc(da);
    hm_dealloc(hm);
    fclose(fp);
}
