#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "da.h"
#include "node.h"

#define not !
#define INIT_CAPACITY 4

char *validate_slice_range(int source_size, int start, int end) {
    char *err =
        source_size == 0       ? "Nothing to slice"
        : start < 0            ? "Start index cannot be negative"
        : start >= source_size ? "Start index cannot be GEQ than size of list"
        : end < 0              ? "End index cannot be negative"
        : end >= source_size   ? "End index cannot be GEQ than size of list"
                               : "";
    return err;
}

int da_malloc_success(DynamicArray *da) {
    if (da == NULL) {
        printf("ERROR: DynamicArray malloc failed\n");
        exit(1);
    }
    return 1;
}
DynamicArray *da__init__(int capacity) {
    DynamicArray *self = malloc(sizeof(DynamicArray));
    da_malloc_success(self);

    if (not capacity) {
        capacity = INIT_CAPACITY;
    }

    self->data = calloc(capacity, sizeof(node *));
    self->size = 0;
    self->capacity = capacity;
    return self;
}

void da__str__(DynamicArray *self) {
    printf("DynamicArray: size=%d, capacity=%d\n", self->size, self->capacity);
    printf("[\n");
    for (int i = 0; i < self->size; ++i) {
        node *n = self->data[i];
        printf("  [%d]: %s : %s\n", i, n->key, n->value);
    }
    printf("]\n");
}

int da_find(DynamicArray *self, char *key) {
    for (int i = 0; i < self->size; ++i) {
        if (not strcmp(self->data[i]->key, key)) {
            return i;
        }
    }
    return -1;
}

node *da_get(DynamicArray *self, int index) {
    if (index < 0 || index >= self->size) {
        return (void *)0;
    }
    return self->data[index];
}

void da_resize(DynamicArray *self, int new_capacity) {
    if (new_capacity <= 0 || new_capacity < self->size) {
        return;
    }
    self->data = realloc(self->data, sizeof(node *) * new_capacity);
    self->capacity = new_capacity;
}

void da_append(DynamicArray *self, node *n) {
    node *new_node = make_node(n->key, n->value);
    if (self->size == self->capacity) {
        da_resize(self, self->capacity * 2);
    }
    ++self->size;
    self->data[self->size - 1] = new_node;
}

void da_insert_at_index(DynamicArray *self, int index, node *n) {
    if (index < 0 || index > self->size) {
        printf("DynamicArray ERROR: attempt to insert out of bounds");
        exit(1);
    }
    da_append(self, n);
    int cursor = self->size - 1;
    while (cursor > index) {
        node temp = *self->data[cursor];
        *self->data[cursor] = *self->data[cursor - 1];
        *self->data[cursor - 1] = temp;
        --cursor;
    }
}

void da_remove_at_index(DynamicArray *self, int index) {
    if (index < 0 || index >= self->size) {
        printf("DynamicArray ERROR: attempt to remove out of bounds");
        exit(1);
    }

    if (self->capacity > 10 && self->size < self->capacity * .25) {
        int doubled = self->size * 2;
        self->capacity = doubled > 10 ? doubled : 10;
    }

    int cursor = index;
    while (cursor < self->size - 1) {
        node temp = *self->data[cursor];
        *self->data[cursor] = *self->data[cursor + 1];
        *self->data[cursor + 1] = temp;
        ++cursor;
    }

    node_dealloc(self->data[self->size - 1]);
    --self->size;
}

void da_update_at_index(DynamicArray *self, int index, node *n) {
    if (index < 0 || index >= self->size) {
        printf("DynamicArray ERROR: attempt to update out of bounds");
        exit(1);
    }
    node *new_node = make_node(n->key, n->value);
    node_dealloc(self->data[index]);
    self->data[index] = new_node;
}

DynamicArray *da_slice(DynamicArray *self, int start, int end) {

    char *err = validate_slice_range(self->size, start, end);
    if (strcmp(err, "")) {
        printf("DynamicArray ERROR: %s\n", err);
        return (void *)0;
    }

    DynamicArray *slice = da__init__(0);

    int direction = start < end ? -1 : 1;
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    int range = end - start;
    int cursor = start;
    while (range != direction) {
        node *curr_node = self->data[cursor];
        da_append(slice, curr_node);
        cursor -= direction;
        range += direction;
    }

    return slice;
}

void da_merge(DynamicArray *self, DynamicArray *other) {
    for (int i = 0; i < other->size; ++i) {
        node *curr = other->data[i];
        da_append(self, curr);
    }
}

void da_dealloc(DynamicArray *self) {

    for (int i = 0; i < self->size; ++i) {
        node_dealloc(self->data[i]);
    }

    free(self->data);
    free(self);
    self = (void *)0;
}
