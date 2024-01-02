#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

char **file_to_str_array(char *input);

int find_char(char *line, char target);

int fopen_success_check(FILE *file);

int get_strlen_of_int(int num);

int int_to_str(int sum, char *sum_str, int sum_strlen);

int max(int a, int b);

int min(int a, int b);

int str_to_int(char *str);

int strstarts(char *str, char *startswith);

#endif // UTIL_H
