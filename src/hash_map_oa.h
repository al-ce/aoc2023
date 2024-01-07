#ifndef HASH_MAP_SC_H
#define HASH_MAP_SC_H

typedef struct HashNode {
    char *key;
    char *value;
    int is_tombstone;
} HashNode;

typedef struct HashMap {
    HashNode **table;
    int capacity;
    int size;
    int hash_function;
} HashMap;

typedef struct ProbeResult {
    int hash_idx;
    HashNode *hn;
    int offset;
} ProbeResult;

HashMap *hm__init__(int capacity, int hash_function);

void hm_dealloc(HashMap *self);

int hm_empty_slots(HashMap *self);

HashNode **hm_get_keys_and_values(HashMap *self);

char *hm_get(HashMap *self, char *key);

int hm_get_size(HashMap *self);

int hm_get_capacity(HashMap *self);

void hm_print_contents(HashMap *self);

void hm_put(HashMap *self, char *key, char *value);

void hm_resize_table(HashMap *self, int capacity);

int hm_contains_key(HashMap *self, char *key);

void hm__str__(HashMap *self);

float hm_table_load(HashMap *self);

int hm_remove(HashMap *self, char *key);

void hm_clear(HashMap *self);

#endif // HASH_MAP_SC_H
