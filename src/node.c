#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node node;

int node_malloc_success(node *np) {
    if (np == 0) {
        printf("ERROR: node malloc failed\n");
        exit(1);
    }
    return 1;
}

void node__str__(node *self) {
    printf("Node { k: %s ", self->key);
    for (int i = 0; i < strlen(self->key); ++i) {
        printf(" ");
    }
    printf("v: %s }\n", self->value);
}

node *node_malloc(void) {
    node *new_node = malloc(sizeof(node));
    node_malloc_success(new_node);
    return new_node;
}

void node_dealloc(node *self) {
    free(self->key);
    free(self->value);
    self->prev = (void *)0;
    self->next = (void *)0;
    self->key = (void *)0;
    self->value = (void *)0;
    free(self);
    self = (void *)0;
}

node *make_node(char *key, char *value) {
    node *new_node = node_malloc();
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    return new_node;
}
