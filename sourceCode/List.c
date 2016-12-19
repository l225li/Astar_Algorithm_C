/********************************************************************
 *
 * $Id: List.c 1989 2015-10-16 18:19:52Z phil $
 *
 ********************************************************************/
/**
 *
 * Creation of a generic (simply linked) List structure.
 *
 * To create a list, one must provide two functions (one function to
 * compare / order elements, one function to display them). Unlike arrays,
 * indices begins with 1.
 *
 ********************************************************************/

#include <stdio.h>
#include "List.h"

/** stores the list of available Nodes instead of deallocating / reallocating
 * them all the time */
static Node* available = 0;



/** Empty List creation by dynamic memory allocation (O(1)).
 * @param comp comparison function between elements (ala strcmp())
 * @param pr display function for list elements
 * @return a new (empty) list if memory allocation OK
 * @return 0 otherwise
 */

List * newList (compFun comp, prFun pr) {
    List * l;
    l = (List*) malloc(sizeof(List));
    if (! l) return 0;
    l->nelts	= 0;
    l->head	= 0;
    l->comp 	= comp;
    l->pr     = pr;
    return l;
}


/** destroy the list by deallocating used memory (O(N)).
 * @param l the list to destroy */
void delList (List* l) {
    Node * tmp = l->head;
    while (tmp) {
        l->head = tmp->next;
        tmp->next = available;
        available = tmp;
        tmp = l->head;
    }
    
    /* tmp == 0 : list is empty */
    
    l->head = 0;
    l->nelts = 0;
    free(l);
}


/** get the Nth element of the list (O(N)).
 * @param l the list
 * @param pos the index of the element in list
 * @param res (out) the searched element
 * @return OK if element found
 * @return ERRINDEX if index out of list bounds
 */

status nthInList (List* l, int pos, void** res) {
    Node * tmp = l->head;
    if (pos <= 0 || pos > l->nelts)
        return ERRINDEX;
    while (pos-- > 1) tmp = tmp->next;
    *res = tmp->val;
    return OK;
}


/** Insert element at a given position in the list (O(N)).
 * @param l the list to store the element in
 * @param pos the position of the insertion point
 * @param elt the element to insert
 * @return ERRINDEX if position is out of list bounds
 * @return ERRALLOC if memory allocation failed
 * @return OK otherwise
 */
status addListAt (List* l, int pos, void* elt) {
    if (pos <= 0 || pos > l->nelts+1)
        return ERRINDEX;
    else {
        
        /* get a new Node and increment length */
        Node * toAdd = available;
        if (!toAdd) toAdd = (Node*) malloc(sizeof(Node));
        else available = available->next;
        if (!toAdd) return ERRALLOC;
        l->nelts++;
        toAdd->val = elt;
        
        /* if pos is 1, must change list head */
        if (pos == 1) {
            toAdd->next = l->head;
            l->head = toAdd;
        }
        
        /* otherwise we need a temporary pointer */
        else {
            Node * tmp = l->head;
            
            /* tmp points to the predecessor */
            while (pos-- > 2) tmp = tmp->next;
            
            /* actual insertion */
            toAdd->next = tmp->next;
            tmp->next = toAdd;
        }
        return OK;
    }
}


/** remove the element located at a given position in list (O(N)).
 * @param l the list to remove the element from
 * @param pos the position of the element to remove
 * @param res (out) the removed element
 * @return ERRINDEX if position is out of list bounds
 * @return OK otherwise
 */
status remFromListAt	(List* l,int pos, void** res) {
    Node *toRem = l->head;
    
    if (pos <= 0 || pos > l->nelts) return ERRINDEX;
    
    /* particular case: must remove list head */
    
    if (pos == 1)
        l->head = toRem->next;
    
    /* otherwise point to predecessor to change links */
    else {
        Node * prec = toRem;
        while (pos-- > 2) prec = prec->next;
        toRem = prec->next;
        prec->next = toRem->next;
    }
    
    *res = toRem->val;
    toRem->next = available;
    available = toRem;
    l->nelts--;
    return OK;
}


/** remove given element from given list (O(N)).
 * implies the user has given a comparison function.
 * @param l the list to remove the element from
 * @param elt the element to remove
 * @return ERRABSENT if element is not found in list
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status remFromList (List* l, void* elt) {
    Node * prec = l->head;
    
    if (l->comp == 0) return ERRUNABLE;
    if (l->nelts == 0) return ERRABSENT;
    if (!(l->comp)(elt,prec->val)) return remFromListAt(l,1,&(prec->val));
    
    /* points to the predecessor */
    
    while (prec->next &&  (l->comp)(elt,prec->next->val))
    {
        prec = prec->next;
    }
    
    /* here
     *  - either we point to the last element
     *  - or next one is the one to remove!
     */
    
    if (! prec->next)
        return ERRABSENT;
    else {
        Node * toRem = prec->next;
        prec->next = toRem->next;
        toRem->next = available;
        available = toRem;
        //free(toRem);
        l->nelts--;
        return OK;
    }
}


/** display list elements as "[ e1 ... eN ]" (O(N)).
 * The display of the element is delegated to the prFun function
 * @param l the list to display.
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status displayList (List * l ) {
    Node * tmp = l->head;
    if (l->pr == 0) return ERRUNABLE;
    printf("[ ");
    while (tmp) {
        (l->pr)(tmp->val);
        putchar(' ');
        tmp = tmp->next;
    }
    putchar(']');
    return OK;
}

/** sequencially call given function with each element of given list (O(NxF)).
 * @param l the list
 * @param f the function
 */
void forEach (List* l, void(*f)(void*)) {
    Node * tmp = l->head;
    while (tmp) {
        (*f)(tmp->val);
        tmp = tmp->next;
    }
}


/** compute and return the number of elements in given list (O(1)).
 * @param l the list
 * @return the number of elements in given list
 */
int lengthList (List* l) { return l->nelts; }


/** private function to get the node preceding the one we're looking for
 * @param e the searched element
 * @return 0 if element is not found (typically, list is empty)
 * @return 1 if element is head of list
 * @return a pointer to the predecessor otherwise
 */
static Node* previous(List* l, void* e) {
    Node * prec = l->head;
    if (l->nelts == 0) return 0;
    if (!(l->comp)(e,prec->val)) return (Node*)1;
    
    /* prec must address element prior to the one to remove */
    while (prec->next && (l->comp)(e,prec->next->val))
        prec = prec->next;
    
    /* here,
     *  - either we address nothing (no such element)
     *  - or we address the element prior to the one we're looking for
     */
    return prec;
}


/** add given element to given list according to compFun function (O(N)).
 * @param l the list (supposedly sorted according to compFun function)
 * @param e the element to add
 * @return ERRALLOC if memory allocation failed
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status addList (List* l, void* e) {
    Node * prec = l->head, *toAdd;
    
    if (l->comp == 0) return ERRUNABLE;
    
    /* add to the head if list is empty, if no comparison function is given
     * or if e < head element
     */
    if (l->nelts == 0 || (l->comp)(e,l->head->val)<0)
        return addListAt(l,1,e);
    
    /* otherwise, get predecessor and link new element just after it */
    while (prec && prec->next && (l->comp)(prec->next->val,e)<0)
        prec = prec->next;
    
    toAdd = available;
    if (!toAdd) toAdd = (Node*) malloc(sizeof(Node));
    else available = available->next;
    if (!toAdd) return ERRALLOC;
    toAdd->next = prec->next;
    toAdd->val = e;
    prec->next = toAdd;
    l->nelts++;
    return OK;
}


/** tests whether the list contains given element (O(N)).
 * @param l the list
 * @param e the searched element
 * @return 0 if element is not found in list
 * @return 1 if element is at the head of the list (no predecessor)
 * @return (a pointer to) the predecessor of the search element otherwise
 */
Node* isInList (List* l, void* e) {
    Node * prec = previous(l,e);
    if (!prec) return 0;
    if (prec == (Node*) 1) return (Node*)1;
    if (prec->next == 0) return 0;
    if (!(l->comp)(e,prec->next->val))
        return prec;
    return 0;
}
