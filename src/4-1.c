#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Set winning numbers per line
#define WINSIZE 10

// Read a number and move cursor to the following character
int get_elem(char *c, FILE *input_file) {
    int num = 0;
    while (isdigit(*c)) {
        num = num * 10 + (*c - '0');
        fread(&*c, sizeof(char), 1, input_file);
    }
    return num;
}

// Skip 'Card ' substring (indeterminate number of spaces)
int get_card_num(char *c, FILE *input_file) {
    while (fread(&*c, sizeof(char), 1, input_file) && !isdigit(*c)) {
    }
    return get_elem(c, input_file);
}

void insert_into_sorted_arr(int *arr, int cursor, int new) {
    arr[cursor] = new;
    while (cursor && arr[cursor - 1] > new) {
        int temp = arr[cursor];
        arr[cursor] = arr[cursor - 1];
        arr[cursor - 1] = temp;
        --cursor;
    }
}

// Return boolean (not idx) representing target's presence in array
int binary_search(int *arr, int left, int right, int target) {
    int mid = left + (right - left) / 2;
    if (left >= right) {
        return 0;
    } else if (arr[mid] == target) {
        return 1;
    } else if (arr[mid] < target) {
        return binary_search(arr, mid + 1, right, target);
    } else {
        return binary_search(arr, left, mid, target);
    }
}

int main(void) {

    FILE *input_file = fopen("./input/4", "r");
    if (!input_file) {
        perror("Attempt to open file failed");
        return EXIT_FAILURE;
    }

    int *wins = calloc(sizeof(int), WINSIZE);
    int w_count = 0;

    int tot_points = 0;
    int is_winning_num = 1;
    int matches = 0;

    char c;
    while (fread(&c, sizeof(char), 1, input_file)) {
        if (c == 'C') {
            get_card_num(&c, input_file);
        }
        else if (c == '|') {
            is_winning_num = 0;
        }
        else if (isdigit(c)) {
            int num = get_elem(&c, input_file);
            // If we're to the left of the |, set nums in winning numbers array
            if (is_winning_num) {
                insert_into_sorted_arr(wins, w_count++, num);
            } else {
                matches += binary_search(wins, 0, w_count, num);
            }
        }

        if (c == '\n') {
            // If matches > 0, add 2^(matches - 1) to the point total
            if (matches) {
                --matches;
                int card_points = 1;
                while (matches--) {
                    card_points *= 2;
                }
                tot_points += card_points;
            }
            // Reset for next card
            for (int i = 0; i < WINSIZE; ++i) {
                wins[i] = 0;
            }
            w_count = 0;
            matches = 0;
            is_winning_num = 1;
        }
    }

    free(wins);
    fclose(input_file);

    printf("\nAOC 2023 4-1\nTot points: %d\n", tot_points);
}
