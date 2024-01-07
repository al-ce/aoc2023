#ifndef NODE_H
#define NODE_H

typedef struct node node;
struct node {
    char *key;
    char *value;
    node *prev;
    node *next;
};

int node_malloc_success(node *np);

node *node_malloc(void);

node *make_node(char *key, char* value);

void node_dealloc(node *self);

void node__str__(node *self);

#endif // NODE_H
