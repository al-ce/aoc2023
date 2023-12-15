#include <ctype.h>
#include <stdio.h>

#include "util.h"

int sum_of_outer_ints(FILE *input) {
    char c;
    int seen = 0, sum = 0, val = 0;
    while (fread(&c,  sizeof(char), 1, input)) {
        if (isdigit(c)) {
            val = c - 48;
        }
        if (!seen && val) {
            sum += val * 10;
            seen = 1;
        } else if (c == 10) {
            sum += val;
            seen = 0;
            val = 0;
        }
    }
    return sum;
};


void write_answer_to_file(FILE *answer, char *sum_str, int sum_strlen) {
    for (int i = 0; i < sum_strlen; i++) {
        const char c[] = {sum_str[i]};
        fputs(c, answer);
    }
}

int main(void) {

    FILE* input = fopen("./input/1", "r");
    fopen_success_check(input);

    int sum = sum_of_outer_ints(input);
    printf("%d\n", sum);

    fclose(input);

    int sum_strlen = get_strlen_of_int(sum);

    char sum_str[sum_strlen];
    int_to_str(sum, sum_str, sum_strlen);

    FILE* answer = fopen("./output/1-1.txt", "w");
    fopen_success_check(answer);
    write_answer_to_file(answer, sum_str, sum_strlen);
    fclose(answer);
}
