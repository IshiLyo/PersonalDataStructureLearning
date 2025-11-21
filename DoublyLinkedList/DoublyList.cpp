#include "DoublyList.h"
#include <cstdlib>

void InitDList(Position &P)
{
	P = NULL;
}

Position FindPre(Position &P)
{
	if (P == NULL)
		return NULL;
	return P->pre;
}

Position FindNext(Position &P)
{
	if (P == NULL)
		return NULL;
	return P->next;
}

bool PreInsert(Position &P, const Item &x)
{
	Node *temp = static_cast<Position>(malloc(sizeof(Node)));
	if (temp == NULL)
		return false;
	temp->item = x;
	if (P == NULL)
	{
		temp->pre = NULL;
		temp->next = NULL;
		P = temp;
	}
	else if (P->pre == NULL)
	{
		temp->pre = NULL;
		temp->next = P;
		P->pre = temp;
	}
	else
	{
		temp->pre = P->pre;
		P->pre->next = temp;
		temp->next = P;
		P->pre = temp;
	}
	return true;
}

bool NextInsert(Position &P, const Item &x)
{
	Node *temp = static_cast<Position>(malloc(sizeof(Node)));
	if (temp == NULL)
		return false;
	temp->item = x;
	if (P == NULL)
	{
		temp->pre = NULL;
		temp->next = NULL;
		P = temp;
	}
	else if (P->next == NULL)
	{
		temp->pre = P;
		temp->next = NULL;
		P->next = temp;
	}
	else
	{
		temp->next = P->next;
		P->next->pre = temp;
		temp->pre = P;
		P->next = temp;
	}
	return true;
}

bool PreDelete(Position &P)
{
	if (P == NULL)
		return false;
	if (P->pre == NULL)
		return false;
	Node *temp = P->pre;
	if (P->pre->pre == NULL)
		P->pre = NULL;
	else
		P->pre->pre->next = P;
	free(temp);
	return true;
}

bool NextDelete(Position &P)
{
	if (P == NULL)
		return false;
	if (P->next == NULL)
		return false;
	Node *temp = P->next;
	if (P->next->next == NULL)
		P->next = NULL;
	else
		P->next->next->pre = P;
	free(temp);
	return true;
}

void ReleaseDlist(Position &P)
{
	if (P == NULL)
		return;
	Node *pre = P->pre;
	Node *temp = P->pre;
	while (pre != NULL)
	{
		pre = pre->pre;
		free(temp);
		temp = pre;
	}

	Node *next = P->next;
	temp = next;
	while (next != NULL)
	{
		next = next->next;
		free(temp);
		temp = next;
	}

	free(P);
	P = NULL;
	return;
}