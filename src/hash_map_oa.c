#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map_oa.h"

#define not !

int is_prime(int n) {
    if (n == 2 || n == 3) {
        return 1;
    }
    if (n <= 1 || n % 2 == 0 || n % 3 == 0) {
        return 0;
    }
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    return 1;
}

// Return the next prime number after n
int next_prime(int n) {
    if (n % 2 == 0) {
        ++n;
    }
    while (not is_prime(n)) {
        n += 2;
    }
    return n;
}

int hash_function_1(char *key) {
    int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash += key[i];
    }
    return hash;
}

int hash_function_2(char *key) {
    int hash = 0;
    int index = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash += (index + 1) * key[i];
        ++index;
    }
    return hash;
}

int hm_malloc_success(HashMap *hmsc) {
    if (hmsc == NULL) {
        printf("ERROR: HashMap malloc failed\n");
        exit(1);
    }
    return 1;
}

void hm_print_contents(HashMap *self) {
    HashNode **keys_and_values = hm_get_keys_and_values(self);
    printf("HashMap Contents:\n");
    printf("----\n");
    for (int i = 0; i < self->size; i++) {
        HashNode *hn = keys_and_values[i];
        printf("  [%d]: %s : %s\n", i, hn->key, hn->value);
    }
    printf("----\n\n");
    free(keys_and_values);
}


HashMap *hm__init__(int capacity, int hash_function) {

    if (capacity < 1) {
        printf("ERROR: HashMap capacity must be > 0 (got %d)\n", capacity);
        return (void *)0;
    } else if (capacity < 3) {
        capacity = 3;
    }

    HashMap *self = malloc(sizeof(HashMap));
    hm_malloc_success(self);
    capacity = is_prime(capacity) ? capacity : next_prime(capacity);
    self->table = calloc(capacity, sizeof(HashNode *));
    self->capacity = capacity;
    self->size = 0;
    self->hash_function = hash_function;
    return self;
}

void free_table(HashMap *self) {
    for (int i = 0; i < self->capacity; i++) {
        HashNode *hn = self->table[i];
        if (hn) {
            free(hn->key);
            free(hn->value);
            hn->key = (void *)0;
            hn->value = (void *)0;
            free(hn);
            hn = (void *)0;
        }
    }
    free(self->table);
    self->table = (void *)0;
}

void hm_dealloc(HashMap *self) {
    free_table(self);
    free(self);
    self = (void *)0;
};

int hm_get_size(HashMap *self) { return self->size; }

int hm_get_capacity(HashMap *self) { return self->capacity; }

int hm_empty_slots(HashMap *self) { return self->capacity - self->size; }

float hm_table_load(HashMap *self) {
    return (float)self->size / (float)self->capacity;
}

HashNode **hm_get_keys_and_values(HashMap *self) {
    HashNode **keys_and_values = malloc(sizeof(HashNode *) * self->size);
    int cursor = 0;
    for (int i = 0; i < self->capacity; i++) {
        HashNode *hn = self->table[i];
        if (hn && not hn->is_tombstone) {
            keys_and_values[cursor] = hn;
            ++cursor;
        }
    }
    return keys_and_values;
}

void hm_resize_table(HashMap *self, int capacity) {
    if (capacity < self->size) {
        return;
    }

    capacity = next_prime(capacity);

    HashNode **old_table = self->table;
    int old_capacity = self->capacity;
    self->capacity = capacity;
    self->size = 0;
    self->table = calloc(self->capacity, sizeof(HashNode *));

    for (int i = 0; i < old_capacity; i++) {
        HashNode *hn = old_table[i];
        if (hn && not hn->is_tombstone) {
            hm_put(self, hn->key, hn->value);
            free(hn->key);
            free(hn->value);
            hn->key = (void *)0;
            hn->value = (void *)0;
            free(hn);
            hn = (void *)0;
        }
    }
    free(old_table);
}

int hm_get_hash_idx(HashMap *self, char *key, int offset) {
    int hash;
    if (self->hash_function == 1) {
        hash = hash_function_1(key);
    } else {
        hash = hash_function_2(key);
    }
    return (hash + (offset * offset)) % self->capacity;
}

ProbeResult hm_probe(HashMap *self, char *key) {
    int offset = 0;
    int hash_idx = hm_get_hash_idx(self, key, offset);
    HashNode *hn = self->table[hash_idx];
    while (hn) {
        if (not strcmp(hn->key, key)) {
            ProbeResult pr = {hash_idx, hn, offset};
            return pr;
        }
        ++offset;
        hash_idx = hm_get_hash_idx(self, key, offset);
        hn = self->table[hash_idx];
    }
    ProbeResult pr = {hash_idx, hn, offset};
    return pr;
}

void hm_put(HashMap *self, char *key, char *value) {

    char *put_value = strdup(value);

    if (hm_table_load(self) >= 0.5) {
        hm_resize_table(self, self->capacity * 2);
    }

    ProbeResult pr = hm_probe(self, key);
    if (pr.hn) {
        free(pr.hn->value);
        pr.hn->value = put_value;
        self->size += pr.hn->is_tombstone;
        pr.hn->is_tombstone = 0;
        return;
    }

    int offset = pr.offset;
    int hash_idx = hm_get_hash_idx(self, key, offset);
    HashNode *new_hn = malloc(sizeof(HashNode));
    char *put_key = strdup(key);
    new_hn->key = put_key;
    new_hn->value = put_value;
    new_hn->is_tombstone = 0;
    self->table[hash_idx] = new_hn;
    ++self->size;
}

int hm_contains_key(HashMap *self, char *key) {
    if (self->size == 0) {
        return 0;
    }
    ProbeResult pr = hm_probe(self, key);
    if (pr.hn) {
        return 1;
    }
    return 0;
}

char *hm_get(HashMap *self, char *key) {
    ProbeResult pr = hm_probe(self, key);
    if (pr.hn) {
        return pr.hn->value;
    }
    return (void *)0;
}

int hm_remove(HashMap *self, char *key) {

    int offset = 0;

    int hash_idx = hm_get_hash_idx(self, key, offset);
    HashNode *hn = self->table[hash_idx];
    while (hn) {
        if (strcmp(hn->key, key)) {
            ++offset;
            hash_idx = hm_get_hash_idx(self, key, offset);
            hn = self->table[hash_idx];
        } else if (hn->is_tombstone) {
            return 0;
        } else {
            hash_idx = hm_get_hash_idx(self, key, offset);
            self->table[hash_idx]->is_tombstone = 1;
            --self->size;
            return 1;
        }
    }
    return 0;
}

void hm_clear(HashMap *self) {
    free_table(self);
    self->table = calloc(self->capacity, sizeof(HashNode *));
    self->size = 0;
}
