#ifndef MIRROR_H
#define MIRROR_H
#include <stdlib.h>
#include <inttypes.h>

typedef int64_t DATA;
typedef size_t LENGTH;

typedef struct _Node {
    DATA data;
    struct _Node* next;
} Node;

typedef struct _Mirror {
    LENGTH n;
    Node* head;
    bool reverse;
} Mirror;


#endif