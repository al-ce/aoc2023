#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARS 12

const char high_cards[] = {'T', 'J', 'Q', 'K', 'A'};

typedef struct {
    long score;
    long bid;
} value;

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
    if (isdigit(card)) {
        return card - '0';
    }
    for (int i = 0; i < 5; ++i) {
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

int update_hand_type(int count, int type) {
    return count == 5                ? 7 // five of a kind
           : count == 4              ? 6 // quads
           : count == 3 && type == 3 ? 5 // full house (*)
           : count == 2 && type == 4 ? 5 // full house
           : count == 3              ? 4 // three of a kind
           : count == 2 && type == 2 ? 3 // two pair
           : count == 2              ? 2 // one pair
                                     : type;          // high card
}

void sort_hands_by_value(value *hand_values, long hands_played) {
    for (int i = 0; i < hands_played; ++i) {
        for (int j = i; j >= 1; --j) {
            value h1 = hand_values[j];
            value h2 = hand_values[j - 1];
            long s1 = h1.score;
            long s2 = h2.score;
            if (s1 < s2) {
                value temp = h1;
                hand_values[j] = h2;
                hand_values[j - 1] = temp;
            }
        }
    }
}

long get_total_winnings(value *hand_values, long hands_played) {
    long tot = 0;
    for (int i = 0; i < hands_played; ++i) {
        tot += (i + 1) * hand_values[i].bid;
    }
    return tot;
}

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
        char *cards_str = strtok(line, " ");
        int card_count[13 + 2] = {0}; // 1-indexed
        int type = 0;
        long score = 0;

        for (int i = 0; i < 5; ++i) {
            int card_val = get_card_val(cards_str[i]);
            ++card_count[card_val];
            int count = card_count[card_val];
            score = update_score(score, i, card_val);
            type = update_hand_type(count, type);
        }
        score += type * 10000000;

        long bid = strtol(strtok(NULL, "\n"), NULL, 10);
        value s = {.score = score, .bid = bid};

        scores[counter] = s;
        ++counter;
    }

    sort_hands_by_value(scores, hands_played);

    long tot = get_total_winnings(scores, hands_played);
    printf("\nAoC 2023 - Day 7 Part 1\n");
    printf("\nTotal winnings: %lu\n", tot);

    fclose(input_file);
}
