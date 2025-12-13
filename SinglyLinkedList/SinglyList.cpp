// List.cpp to realize the List structure in cpp.
#include "SinglyList.h"
#include <cstdlib>

static List findTail(const List &H)
{
	if (H == NULL)
		return NULL;

	List result = H;
	while (result->next != NULL)
		result = result->next;

	return result;
}

void InitList(List &H)
{
	H = NULL;
}

void ApplyAll(List &H, void(*Fuc)(Item))
{
	List temp = H;
	while (temp != NULL)
	{
		Fuc(temp->item);
		temp = temp->next;
	}
}

bool AddToHead(List &H, const Item &x)
{
	List temp = static_cast<List>(malloc(sizeof(Node)));

	if (temp == NULL)
		return false;

	temp->item = x;
	temp->next = H;
	H = temp;

	return true;
}

int AddToNth(List &H, const Item &x, int n)
{
	List temp = static_cast<List>(malloc(sizeof(Node)));

	if (temp == NULL)
		return -1;

	temp->item = x;

	int count = CountList(H);

	if (n < 0 || count <= n)
	{
		free(temp);
		return -2;
	}

	if (n == 0)
	{
		temp->next = H;
		H = temp;
	}
	else
	{
		List cur = H;
		for (int i = 0; i < n - 1; ++i)
			cur = cur->next;

		temp->next = cur->next;
		cur->next = temp;
	}

	return n;
}

bool AddToEnd(List &H, const Item &x)
{
	List temp = static_cast<List>(malloc(sizeof(Node)));

	if (temp == NULL)
		return false;

	temp->item = x;
	temp->next = NULL;

	if (H == NULL)
		H = temp;
	else
		findTail(H)->next = temp;

	return true;
}

bool DelHead(List &H)
{
	if (H == NULL)
		return false;

	List temp = H;
	H = H->next;

	free(temp);

	return true;
}

bool DelNth(List &H, int n)
{
	int count = CountList(H);

	if (n < 0 || count < n)
		return false;

	List cur = H;
	for (int i = 0; i < n - 1; ++i)
		cur = cur->next;

	if (cur == NULL)
		return false;

	if (n == 0)
		DelHead(H);
	else
	{
		List temp = cur->next;
		cur->next = cur->next->next;

		free(temp);
	}

	return true;
}

bool DelTail(List &H)
{
	if (H == NULL)
		return false;

	List cur = H;
	List pre = H;

	while (cur != NULL && cur->next != NULL)
	{
		pre = cur;
		cur = cur->next;
	}

	pre->next = NULL;
	free(cur);

	return true;
}

int CountList(const List &H)
{
	int count = 0;

	List point = H;
	while (point != NULL)
	{
		++count;
		point = point->next;
	}

	return count;
}

int ReleaseList(List &H)
{
	List temp = H;
	int count = 0;

	while (temp != NULL)
	{
		++count;
		H = H->next;
		free(temp);
		temp = H;
	}

	H = NULL;

	return count;
}