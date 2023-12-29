#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Set winning numbers per line
#define WINSIZE 10
#define CARDSCOUNTSIZE 199

// Read a number and move the read_ptr to the following character
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

// For each current copy, make copies for the next {matches} number of cards
void make_card_copies(int *cards_won, int card_num, int matches) {
    for (int j = card_num + 1; j <= card_num + matches; ++j) {
        cards_won[j] += cards_won[card_num];
    }
}

int main(void) {

    FILE *input_file = fopen("./input/4", "r");
    if (!input_file) {
        perror("Attempt to open file failed");
        return EXIT_FAILURE;
    }

    int *cards_count = calloc(sizeof(int), CARDSCOUNTSIZE);

    int *wins = calloc(sizeof(int), WINSIZE);
    int w_count = 0;

    int is_winning_num = 1;
    int matches = 0;

    char c;
    int card_num = 0;
    while (fread(&c, sizeof(char), 1, input_file)) {
        if (c == 'C') {
            card_num = get_card_num(&c, input_file);
        } else if (c == '|') {
            is_winning_num = 0;
        } else if (isdigit(c)) {
            int num = get_elem(&c, input_file);
            if (is_winning_num) {
                insert_into_sorted_arr(wins, w_count++, num);
            } else {
                matches += binary_search(wins, 0, w_count, num);
            }
        }

        if (c == '\n') {
            ++cards_count[card_num]; // Account for original card
            make_card_copies(cards_count, card_num, matches);

            // Reset for next card
            for (int i = 0; i < WINSIZE; ++i) {
                wins[i] = 0;
            }
            w_count = 0;
            matches = 0;
            is_winning_num = 1;
        }
    }

    int tot = 0;
    for (int i = 0; i < CARDSCOUNTSIZE; ++i) {
        tot += cards_count[i];
    }

    printf("\nAOC 2023 4-2\n");
    printf("Cards won: %d\n", tot);

    free(wins);
    free(cards_count);
    fclose(input_file);
}
