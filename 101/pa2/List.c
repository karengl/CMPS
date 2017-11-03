// Karen Lee
// kglee
// pa2
// List.c
// Implements List ADT from List.h. Defines all functions and structs.

#include <stdio.h>
#include <stdlib.h>

#include "List.h"

// structs --------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj {
    int data;
    struct NodeObj* prev;
    struct NodeObj* next;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// private ListObj type
typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;

// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes prev, next, and data fields.
// Private.
Node newNode(int data, Node prev, Node next) {
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->prev = prev;
    N->next = next;
    return N;
}

// freeNode()
// Frees heap memory pointed to by *pN, set *pN to NULL.
// Private.
void freeNode(Node* pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

// newList()
// Returns reference to new empty List object.
List newList(void) {
    List L;
    L = malloc(sizeof(ListObj));
    L->front = L->back = NULL;
    L->length = 0;
    L->index = -1;
    return L;
}

// freeList()
// Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        Node tmp = (*pL)->front;
        while (tmp != NULL) {
            Node curr = tmp;
            tmp = tmp->next;
            free(curr);
        }
        free(*pL);
        *pL = NULL;
    }
}

// Access functions -----------------------------------------------------------

// length()
// Returns the number of elements in this List.
int length(List L) {
    return L->length;
}

// index()
// If cursor is defined, returns the index of the cursor element,
// otherwise returns -1.
int index(List L) {
    return L->index;
}

// front()
// Returns front element. Pre: length()>0
int front(List L) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: front() called on empty List\n");
        exit(1);
    }
    return L->front->data;
}

// back()
// Returns back element. Pre: length()>0
int back(List L) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: back() called on empty List\n");
        exit(1);
    }
    return L->back->data;
}

// get()
// Returns cursor element. Pre: length()>0, index()>=0
int get(List L) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: get() called on empty List\n");
        exit(1);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: get() called on undefined index\n");
        exit(1);
    }
    return L->cursor->data;
}

// equals()
// Returns true if List A and B are the same integer sequence.
// The cursor is ignored in both lists.
int equals(List A, List B) {
    if (A->length != B->length)
        return 0;
    Node Atmp = A->front;
    Node Btmp = B->front;
    while (Atmp != NULL && Btmp != NULL) {
        if (Atmp->data != Btmp->data)
            return 0;
        Atmp = Atmp->next;
        Btmp = Btmp->next;
    }
    return 1;
}

// Manipulation procedures ----------------------------------------------------

// clear()
// Resets this List to its original empty state.
void clear(List L) {
    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
    L->length = 0;
    L->index = -1;
}

// moveFront()
// If List is non-empty, places the cursor under the front element,
// otherwise does nothing.
void moveFront(List L) {
    if (length > 0) {
        L->cursor = L->front;
        L->index = 0;
    }
}

// moveBack()
// If List is non-empty, places the cursor under the back element,
// otherwise does nothing.
void moveBack(List L) {
    if (length > 0) {
        L->cursor = L->back;
        L->index = L->length - 1;
    }
}

// movePrev()
// If cursor is defined and not at front, moves cursor one step toward
// front of this List, if cursor is defined and at front, cursor becomes
// undefined, if cursor is undefined does nothing.
void movePrev(List L) {
    if (L->cursor != NULL) {
        if (L->index != 0) {
            L->cursor = L->cursor->prev;
            L->index = L->index - 1;
        }else {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}

// moveNext()
// If cursor is defined and not at back, moves cursor one step toward
// back of this List, if cursor is defined and at back, cursor becomes
// undefined, if cursor is undefined does nothing.
void moveNext(List L) {
    if (L->cursor != NULL) {
        if (L->index != L->length-1) {
            L->cursor = L->cursor->next;
            L->index = L->index + 1;
        }else {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}

// prepend()
// Insert new element into this List. If List is non-empty,
// insertion takes place before first element.
void prepend(List L, int data) {
    Node tmp = newNode(data, NULL, L->front);
    if (L->length == 0)
        L->back = tmp;
    else
        L->front->prev = tmp;
    L->front = tmp;
    L->length = L->length + 1;
}

// append()
// Insert new element into this List. If List is non-empty,
// insertion takes place after back element.
void append(List L, int data) {
    Node tmp = newNode(data, L->back, NULL);
    if (L->length == 0)
        L->front = tmp;
    else
        L->back->next = tmp;
    L->back = tmp;
    L->length++;
}

// insertBefore()
// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, int data) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: insertBefore() called on empty List\n");
        exit(1);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: insertBefore() called on undefined index\n");
        exit(1);
    }
    Node tmp = newNode(data, L->cursor->prev, L->cursor);
    if (L->index == 0)
        L->front = tmp;
    else
        L->cursor->prev->next = tmp;
    L->cursor->prev = tmp;
    L->index++;
    L->length++;
}

// insertAfter()
// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, int data) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: insertAfter() called on empty List\n");
        exit(1);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: insertAfter() called on undefined index\n");
        exit(1);
    }
    Node tmp = newNode(data, L->cursor, L->cursor->next);
    if (L->index == L->length-1)
        L->back = tmp;
    else
        L->cursor->next->prev = tmp;
    L->cursor->next = tmp;
    L->index++;
    L->length++;
}

// deleteFront()
// Deletes the front element. Pre: length()>0
void deleteFront(List L) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: deleteFront() called on empty List\n");
        exit(1);
    }
    L->front = L->front->next;
    L->front->prev = NULL;
    L->length--;
    if (L->index == 0)
        L->cursor = NULL;
    if (L->index > -1)
        L->index--;
}

// deleteBack()
// Deletes the back element. Pre: length()>0
void deleteBack(List L) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: deleteBack() called on empty List\n");
        exit(1);
    }
    L->back = L->back->prev;
    L->back->next = NULL;
    L->length--;
    if (L->index == L->length-1) {
        L->cursor = NULL;
        L->index = -1;
    }
}

// delete()
// Deletes the cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L) {
    if (L->length < 1) {
        fprintf(stderr, "List Error: delete() called on empty List\n");
        exit(1);
    }
    if (L->index < 0) {
        fprintf(stderr, "List Error: delete() called on undefined index\n");
        exit(1);
    }
    L->cursor->prev->next = L->cursor->next;
    L->cursor->next->prev = L->cursor->prev;
    L->index = -1;
    L->length--;
}

// Other operations -----------------------------------------------------------

// printList()
// Prints data elements in L to FILE out
void printList(FILE* out, List L) {
    Node tmp = L->front;
    while (tmp != NULL) {
        fprintf(out, "%d ", tmp->data);
        tmp = tmp->next;
    }
}

// copyList()
// Returns a new List representing the same integer sequence as this
// List. The cursor in the new list is undefined, regardless of the
// state of the cursor in this List. This List is unchanged.
List copyList(List L) {
    List copy = newList();
    Node tmp = L->front;
    while (tmp != NULL) {
        append(copy, tmp->data);
        tmp = tmp->next;
    }
    return copy;
}
