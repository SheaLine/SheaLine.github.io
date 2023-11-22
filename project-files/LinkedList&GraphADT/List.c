// ----------------------------------------------------------------------------
// Shea Line
// sline
// 2023 Winter CSE101 pa1
// ----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "List.h"

// structs --------------------------------------------------------------------

// private Nodeobj Type
typedef struct Nodeobj
{
	int data;
	struct Nodeobj* prev;
	struct Nodeobj* next;
}Nodeobj;

typedef struct Nodeobj* Node;

typedef struct ListObj
{
	Node front;
	Node back;
	Node cursor;
	int length;
	int index;
}ListObj;

// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
Node newNode(int data)
{
	Node N = malloc(sizeof(Nodeobj));
	assert( N!=NULL );
	N->data = data;
	N->next =  NULL;
	N->prev = NULL;
	return N;
}
// feeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN)
{
	if (pN != NULL && *pN != NULL)
	{
		free(*pN);
		*pN = NULL;
	}
}
// newList()
// Returns reference to new empty List object.
List newList(void)
{
	List L;
	L = malloc(sizeof(ListObj));
	assert( L!=NULL );
	L->front = L->back = L->cursor = NULL;
	L->length = 0;
	L->index = -1;
	return L;
}
// freeList()
// Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL)
{
	if (pL != NULL && *pL != NULL)
	{
		clear(*pL);
		free(*pL);
		*pL = NULL;
	}
}

// Access functions -----------------------------------------------------------

// length()
// Returns the number of elements in L.
int length(List L)
{
	if( L==NULL ){
      	fprintf(stderr, "%s", "List Error: calling Length() on NULL List reference\n");
      	exit(EXIT_FAILURE);
   }
   return L->length;
}
// index()
// Returns index of cursor element if defined, -1 otherwise.
int index(List L)
{
	if( L==NULL ){
		fprintf(stderr, "%s", "List Error: calling index() on NULL List reference\n");
      	exit(EXIT_FAILURE);
   }
   return L->index;
}
// front()
// Returns front element of L. Pre: length()>0
int front(List L)
{
	if (length(L) == 0){
		fprintf(stderr, "%s", "List Error: calling front() on Empty List reference\n");
		exit(EXIT_FAILURE);
	}
	return L->front->data;
}
// back()
// Returns back element of L. Pre: length()>0
int back(List L)
{
	if (length(L) == 0){
		fprintf(stderr, "%s", "List Error: calling back() on Empty List reference\n");
		exit(EXIT_FAILURE);
	}
	return L->back->data;
}
// get()
// Returns cursor element of L. Pre: length()>0, index()>=0
int get(List L)
{
	if (length(L) == 0 && index(L) < 0)
	{
		fprintf(stderr, "%s", "List Error: calling get() on Empty List reference\n");
		exit(EXIT_FAILURE);
	}
	return L->cursor->data;
}
// equals()
// Returns true if Lists A and B are in same state, and returns false otherwise.
bool equals(List A, List B)
{
	if (A == NULL || B == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling equals() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (A->length != B->length)
	{
		return false;
	}
	/*
	if (A->index != B->index)
	{
		return false;
	}
	*/
	Node copy_a = A->front;
	Node copy_b = B->front;
	while(copy_a != NULL && copy_b != NULL)
	{
		if (copy_a->data != copy_b->data)
			return false;
		copy_a = copy_a->next;
		copy_b = copy_b->next;		
	}
	return true;
}

// Manipulation procedures -------------------------------------------------------

// clear() 
// Resets L to its original empty state.
void clear(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling clear() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	while (length(L) > 0)
		deleteFront(L);
	L->cursor = NULL;
	L->index = -1;
}
// set()
// Overwrites the cursor element's data with x. Pre: length()>0, index()>=0
void set(List L, int x)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling set() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (length(L) <=0 || index(L) < 0)
	{
		fprintf(stderr, "%s", "List Error: calling set() on an empty List or undefined cursor\n");
		exit(EXIT_FAILURE);
	}
	L->cursor->data = x;
}
// moveFront()
// If L is non-empty, sets cursor under the front element, otherwise does nothing.
void moveFront(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling moveFront() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (length(L) > 0)
	{
		L->cursor = L->front;
		L->index = 0;
	}
}
// moveBack()
// If L is non-empty, sets cursor under the back element, otherwise does nothing.
void moveBack(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling moveBack() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (length(L) > 0)
	{
		L->cursor = L->back;
		L->index = length(L) - 1;
	}
}

// movePrev()
// If cursor is defined and not at front, move cursor one step toward the front of L
// if cursor is defined and at front, cursor becomes undefined;
// if cursor is undefined do nothing
void movePrev(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling movePrev() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (L->cursor != NULL && L->cursor != L->front)
	{
		L->cursor = L->cursor->prev;
		L->index--;
	}
	else if (L->cursor == L->front)
	{
		L->cursor = NULL;
		L->index = -1;
	}
}
// moveNext()
// If cursor is defined and not at back, move cursor one step toward the back of L;
// if cursor is defined and at back, cursor becomes undefined
// if cursor is undefined do nothing
void moveNext(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling moveNext() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (L->cursor != NULL && L->cursor != L->back)
	{
		L->cursor = L->cursor->next;
		L->index++;
	}
	else if (L->cursor == L->back)
	{
		L->cursor = NULL;
		L->index = -1;
	}
}
// prepend()
// Insert new element into L
// If L is non-empty, insertion takes place before front element.
void prepend(List L, int x)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling prepend() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	Node new_Node = newNode(x);
	if (length(L) == 0)
	{
		L->front = L->back = new_Node;
	}
	else
	{
		new_Node->next = L->front;
		new_Node->prev = NULL;
		L->front->prev = new_Node;
		L->front = new_Node;
		if (L->index != -1)
		{
			L->index++;
		}
	}
	L->length++;
}
// append()
// Insert new element into L. If L is non-empty, insertion takes place after back element.
void append(List L, int x)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling append() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	Node new_Node = newNode(x);
	new_Node->next = NULL;
	if (length(L) == 0)
	{
		L->back = L->front = new_Node;
		new_Node->prev = NULL;
	}
	else
	{
		L->back->next = new_Node;
		new_Node -> prev = L->back;
		L->back = new_Node;
	}
	/*
	if (L->index != -1)
	{
		L->index++;
	}
	*/
	L->length++;
}
// insertBefore()
// Insert new element before cursor. Pre: length()>0, index()>=0
void insertBefore(List L, int x)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling insertBefore() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (length(L) <=0 || index(L) < 0)
	{
		fprintf(stderr, "%s", "List Error: calling insertBefore() on an empty List or undefined cursor\n");
		exit(EXIT_FAILURE);
	}
	Node new_Node = newNode(x);
	new_Node->next = L->cursor;
	new_Node->prev = L->cursor->prev;
	if (L->cursor->prev != NULL)
		L->cursor->prev->next = new_Node;
	else
		L->front = new_Node;
	L->cursor->prev = new_Node;
	L->length++;
	L->index++;
}
// insertAfter()
// Insert new element after cursor. Pre: length()>0, index()>=0
void insertAfter(List L, int x)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling insertAfter() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (length(L) <=0 || index(L) < 0)
	{
		fprintf(stderr, "%s", "List Error: calling insertAfter() on an empty List or undefined cursor\n");
		exit(EXIT_FAILURE);
	}
	Node new_Node = newNode(x);
	new_Node->prev = L->cursor;
	new_Node->next = L->cursor->next;
	if (L->cursor->next != NULL)
		L->cursor->next->prev = new_Node;
	else
		L->back = new_Node;
	L->cursor->next = new_Node;
	L->length++;
}
// deleteFront()
// Delete the front element. Pre: length()>0
void deleteFront(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling deleteFront() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (length(L) <= 0) 
	{
        fprintf(stderr, "%s","List Error: calling deleteFront() on an empty List reference\n");
        exit(EXIT_FAILURE);
    }
    Node temp = L->front;
    if (L->cursor == L->front)
    {
    	L->cursor = NULL;
    	L->index = -1;
    }
    else
    	L->index--;
    L->front=L->front->next;
    if (L->front != NULL)
    	L->front->prev = NULL;
    else
    	L->back = NULL;
    freeNode(&temp);
    L->length--;
}
// deleteBack()
// Delete the back element. Pre: length()>0
void deleteBack(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling deleteBack() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (length(L) <= 0) 
	{
        fprintf(stderr, "%s","List Error: calling deleteBack() on an empty List reference\n");
        exit(EXIT_FAILURE);
    }
    Node temp = L->back;
    if (L->cursor == L->back)
    {
    	L->cursor = NULL;
    	L->index = -1;
    }
    L->back = L->back->prev;
    if (L->back != NULL)
    	L->back->next = NULL;
    else
    	L->front = NULL;
    freeNode(&temp);
    L->length--;
}
// delete()
// Delete cursor element, making cursor undefined. Pre: length()>0, index()>=0
void delete(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling delete() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	if (L->cursor == NULL || L->index < 0)
	{
		fprintf(stderr, "%s", "List Error: calling delete() on List reference with undefined cursor\n");
		exit(EXIT_FAILURE);
	}
	if (L->cursor == L->front)
		deleteFront(L);
	else if (L->cursor == L->back)
	{
		deleteBack(L);
	}
	else
	{
		Node deleteNode = L->cursor;
		L->cursor->prev->next = L->cursor->next;
		L->cursor->next->prev = L->cursor->prev;
		L->length--;
		freeNode(&deleteNode);
	}

	L->cursor = NULL;
	L->index = -1;
}

// Other operations --------------------------------------------------------------

// printlist()
// Prints to the file pointed to by out, a string representation of L consisting
// of a space separated sequence of integers, with front on left.
void printList(FILE* out, List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling delete() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	Node currentNode = L->front;
	while (currentNode != NULL)
	{
		fprintf(out, "%d ", currentNode->data);
		currentNode = currentNode->next;
	}
}
// copyList()
// Returns a new List representing the same integer sequence as L.
// The cursor in the new list is undefined, regardless of the state of the cursor in L.
// The state of L is unchanged.
List copyList(List L)
{
	if (L == NULL)
	{
		fprintf(stderr, "%s", "List Error: calling delete() on NULL List reference\n");
		exit(EXIT_FAILURE);
	}
	List new_List = newList();
	Node currentNode = L->front;
	while (currentNode != NULL)
	{
		append(new_List, currentNode->data);
		currentNode = currentNode->next;
	}
	return new_List;
}