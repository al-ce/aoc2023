#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARS 12
#define JOKER 'J'
#define JOKER_VAL 1

const char high_cards[] = {'T', 'Q', 'K', 'A'}; // 'J' is 1 in this version

enum hand_type {
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    QUADS,
    FIVE_OF_A_KIND,
};

typedef struct value {
    long score;
    long bid;
} value;

long get_file_len(FILE *file);
long get_card_val(char card);
long update_score(long score, int idx, int card_val);
int update_hand_type(int count, int type, int card_val);
int apply_jokers(int j_count, int type);
void sort_hands_by_score(value *hand_values, long hands_played);
long get_total_winnings(value *hand_values, long hands_played);

int main(void) {
    FILE *input_file = fopen("./input/7", "r");
    if (!input_file) {
        perror("Attempt to open file failed");
        return EXIT_FAILURE;
    }

    char line[MAXCHARS];

    long hands_played = get_file_len(input_file);
    value scores[hands_played];

    int counter = 0;
    while (fgets(line, sizeof(line), input_file)) {
        int card_count[13 + 2] = {0}; // 1-indexed
        int type = 0;
        long score = 0;

        char *cards_str = strtok(line, " ");
        for (int i = 0; i < 5; ++i) {
            int card_val = get_card_val(cards_str[i]);
            ++card_count[card_val];
            int count = card_count[card_val];
            score = update_score(score, i, card_val);
            type = update_hand_type(count, type, card_val);
        }

        int j_count = card_count[JOKER_VAL];
        type = apply_jokers(j_count, type);

        score += type * 10000000;

        long bid = strtol(strtok(NULL, "\n"), NULL, 10);
        value s = {.score = score, .bid = bid};

        scores[counter] = s;
        ++counter;
    }

    sort_hands_by_score(scores, hands_played);

    long tot = get_total_winnings(scores, hands_played);
    printf("\nAoC 2023 - Day 7 Part 2\n");
    printf("\nTotal winnings: %lu\n", tot);

    fclose(input_file);
}

long get_file_len(FILE *file) {
    long len = 0;
    char line[MAXCHARS];
    while (fgets(line, sizeof(line), file)) {
        ++len;
    }
    rewind(file);
    return len;
}

long get_card_val(char card) {
    if (card == JOKER) {
        return JOKER_VAL;
    }
    if (isdigit(card)) {
        return card - '0';
    }
    for (int i = 0; i < 4; ++i) {
        if (high_cards[i] == card) {
            return i + 10;
        }
    }
    return -1;
}

long update_score(long score, int idx, int card_val) {
    int factor = 1;
    for (int j = 0; j < 5 - idx; ++j) {
        factor *= 13;
    }
    return score + card_val * factor;
}

int update_hand_type(int count, int type, int card_val) {
    if (card_val == 1) {
        return type;
    }
    return count == 5                              ? FIVE_OF_A_KIND
           : count == 4                            ? QUADS
           : count == 3 && type == TWO_PAIR        ? FULL_HOUSE
           : count == 2 && type == THREE_OF_A_KIND ? FULL_HOUSE
           : count == 3                            ? THREE_OF_A_KIND
           : count == 2 && type == 1               ? TWO_PAIR
           : count == 2                            ? ONE_PAIR
                                                   : type;
}

int apply_jokers(int j_count, int type) {
    return j_count == 0                        ? type
           : j_count == 1 && type != QUADS     ? type + 2 - (type == HIGH_CARD)
           : j_count == 2 && type == HIGH_CARD ? THREE_OF_A_KIND
           : j_count == 2 && type == ONE_PAIR  ? QUADS
           : j_count == 3 && type == HIGH_CARD ? QUADS
                                               : FIVE_OF_A_KIND;
}

void insertion_sort(value *arr, long arr_len) {
    for (int i = 1; i < arr_len; ++i) {
        value curr = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].score > curr.score) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = curr;
    }
}

void sort_hands_by_score(value *arr, long arr_len) {
    /* quick_sort(arr, 0, arr_len - 1); */
    insertion_sort(arr, arr_len);
}

long get_total_winnings(value *hand_values, long hands_played) {
    long tot = 0;
    for (int i = 0; i < hands_played; ++i) {
        tot += (i + 1) * hand_values[i].bid;
    }
    return tot;
}
