#include "Queue.h"

#include <cstdlib>

void InitQueue(Head &H, Tail &T)
{
    H = NULL;
    T = NULL;
}

bool EnQueue(Head &H, Tail &T, Item x)
{
    if (H == NULL)
    {
        H = static_cast<Queue>(malloc(sizeof(Node)));

        if (H == NULL)
            return false;

        T = H;
        return true;
    }

    temp = static_cast<Queue>)malloc(sizeof(Node)));

    if (temp == NULL)
        return false;
    
    temp->next = NULL;
    temp->item = x;
    Tail->next = temp;

    return true;
}

bool DeQueue(Head &H, Tail &T)
{
    if (H == NULL)
        return false;
    
    Queue temp = H;
    H = H->Next;
    free(temp);

    return true;
}

void ReleaseQueue(Head &H, Tail &T)
{
    Queue cur = H;
    while (cur != NULL)
    {
        H = H->next;
        free(H);
        cur = H;
    }
}