#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct
{
    int x;
    int y;
} Item;

typedef struct node_
{
    Item item;
    node_ * next;
} Node;

typedef Node * Queue;
typedef Node * Head;
typedef Node * Tail;

// To initialize a Queue
void InitQueue(Head &, Tail &);

bool EnQueue(Head &, Tail &);

bool DeQueue(Head &, Tail &);

void ReleaseQueue(Head &, Tail &);

#endif