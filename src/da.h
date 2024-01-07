#ifndef DA_H
#define DA_H

#include "node.h"

typedef struct DynamicArray {
    node **data;
    int size;
    int capacity;
} DynamicArray;

DynamicArray *da__init__(int capacity);

void da__str__(DynamicArray *self);

void da_dealloc(DynamicArray *self);

void da_append(DynamicArray *self, node *n);

int da_find(DynamicArray *self, char *key);

node *da_get(DynamicArray *self, int index);

void da_insert_at_index(DynamicArray *self, int index, node *n);

void da_remove_at_index(DynamicArray *self, int index);

void da_update_at_index(DynamicArray *self, int index, node *n);

void da_resize(DynamicArray *self, int new_capacity);

DynamicArray *da_slice(DynamicArray *self, int start, int end);

void da_merge(DynamicArray *self, DynamicArray *other);

#endif // DA_H

