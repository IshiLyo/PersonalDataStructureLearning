#ifndef LIST_H_
#define LIST_H_

// To indicate a limit of Item.name
static const int MAX_ = 60;

typedef struct
{
	char name[MAX_];
	int age;
	double point;
} Item;

typedef struct node_
{
	Item item;
	struct node_ *next;
} Node;

typedef Node *List;

// To create a list
// Before making any oporation on a list
// Make sure it was once initialized first
void InitList(List &);

// Apply a fuction to each element
void ApplyAll(List &, void(*)(Item));

// To add an element behind the head of a list
// Return false when memory allocation failed
bool AddToHead(List &, const Item &);

// To add an elelmet to the Nth(from 0 to count) position of a list
// Normally add it to head
// Return the N when success
// Return -1 to indicate memory allocation failed
// Return -2 to indicate Nth element beyond the list
int AddToNth(List &, const Item &, int = 0);

// To add an element to the end of the list
bool AddToEnd(List &, const Item &);

// To delete the head member
// Return false if there is nothing to delete
bool DelHead(List &);

// To delete the Nth member(counting from 0)
// Nomally delete the head member
// Return false if there is nothing to delete
bool DelNth(List &, int = 0);

// To delete the tail member
// Return false if there is nothing to delete
bool DelTail(List &);

// To count how many elements there are in the list
int CountList(const List &);

// To release a list
int ReleaseList(List &);

#endif