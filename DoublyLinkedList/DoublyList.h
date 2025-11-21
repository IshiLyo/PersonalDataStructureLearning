#ifndef DOUBLYLIST_H_
#define DOUBLYLIST_H_

typedef struct
{
	int x;
	int y;
} Item;

typedef struct node_
{
	node_ *pre;
	Item item;
	node_ *next;
} Node;

typedef Node *Position;

// To create a doubly list
void InitDList(Position &);

// To find the previous element
Position FindPre(Position &);

// To find the next element
Position FindNext(Position &);

//
bool PreInsert(Position &, const Item &);

bool NextInsert(Position &, const Item &);

bool PreDelete(Position &);

bool NextDelete(Position &);

void ReleaseDlist(Position &);
#endif