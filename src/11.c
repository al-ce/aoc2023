#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long row;
    long col;
} Galaxy;

typedef struct {
    long steps;
    long emptiness;
} Path;

long get_side_len(FILE *fp) {
    long side_len = 0;
    char c;
    while (fread(&c, sizeof(char), 1, fp)) {
        if (c == '\n') {
            break;
        }
        side_len++;
    }
    rewind(fp);
    return side_len;
}

Path get_path(Galaxy g1, Galaxy g2, long *empty_rows, long *empty_cols) {
    long col_delta = g2.col > g1.col ? 1 : -1;
    long row_delta = g2.row > g1.row ? 1 : -1;

    long steps = 0;
    long emptiness = 0;
    for (long k = g1.col; k != g2.col; k += col_delta) {
        steps++;
        emptiness += empty_cols[k];
    }
    for (long k = g1.row; k != g2.row; k += row_delta) {
        steps++;
        emptiness += empty_rows[k];
    }

    return (Path){steps, emptiness};
}

int main(void) {

    FILE *fp = fopen("./input/11", "r");
    long side_len = get_side_len(fp);

    // Track empty rows and columns (assume empty until a galaxy is found)
    long empty_rows[side_len];
    long empty_cols[side_len];
    for (long i = 0; i < side_len; i++) {
        empty_cols[i] = 1;
        empty_rows[i] = 1;
    }

    // Track coordinates of galaxies in a dynamic array
    long gal_len = 0;
    long gal_cap = 2;
    Galaxy *gal_arr = calloc(sizeof(Galaxy), gal_cap);

    // Scan the file for galaxies
    char c;
    long row = 0, col = 0;
    while (fread(&c, sizeof(char), 1, fp)) {
        if (c == '\n') {
            row++;
            col = 0;
            continue;
        }
        if (c == '#') {
            // Found a galaxy, so mark row and column as non-empty
            empty_cols[col] = 0;
            empty_rows[row] = 0;

            ++gal_len;
            if (gal_len == gal_cap) {
                gal_cap *= 2;
                gal_arr = realloc(gal_arr, sizeof(Galaxy) * gal_cap);
            }
            gal_arr[gal_len - 1] = (Galaxy){row, col};
        }
        col++;
    }

    // Track which distances between galaxies have already been calculated
    long pairings[gal_len][gal_len];
    for (long i = 0; i < gal_len; i++) {
        for (long j = 0; j < gal_len; j++) {
            pairings[i][j] = 0;
        }
    }

    long expansion = 0;
    long distance_sum = 0;
    for (long i = 0; i < gal_len; i++) {
        for (long j = 0; j < gal_len; j++) {
            if (pairings[i][j] || pairings[j][i]) {
                continue;
            }
            pairings[i][j] = 1;

            Galaxy g1 = gal_arr[i];
            Galaxy g2 = gal_arr[j];
            Path path = get_path(g1, g2, empty_rows, empty_cols);
            distance_sum += path.steps;
            expansion += path.emptiness;
        }
    }

    long ancient_distance_sum = distance_sum + expansion * 999999;
    long young_distance_sum = distance_sum + expansion;

    printf("\nAoC 2023 Day 11\n");
    printf("Total distance between all galaxies:\n");

    printf("Part One (young universe)  : %ld\n", young_distance_sum);
    printf("Part Two (ancient universe): %ld\n", ancient_distance_sum);

    free(gal_arr);
    fclose(fp);
}
