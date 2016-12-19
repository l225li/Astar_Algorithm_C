/********************************************************************
 *
 * $Id: List.h 1989 2015-10-16 18:19:52Z phil $
 *
 ********************************************************************
 */
/**
 *
 * Creation of a generic (simply linked) List structure.
 *
 * To create a list, one must provide two functions (one function to
 * compare / order elements, one function to display them). Unlike arrays,
 * indices begins with 1.
 *
 ********************************************************************/

#ifndef __List_H
#define __List_H

#include <stdlib.h>
#include "status.h"

/** Typical simple link structure: a Node is a "value / next" pair */
typedef struct Node {
    void		*val;
    struct Node	*next;
} Node;

/** Comparison function for list elements.
 * Must follow the "strcmp" convention: result is negative if e1 is less
 * than e2, null if they are equal, and positive otherwise.
 */
typedef int(*compFun)   (void* e1, void* e2);

/** Display function for list elements */
typedef void(*prFun)   (void*);

/** The list embeds a counter for its size and the two function pointers */
typedef struct List {
    int nelts;
    Node * head;
    compFun comp;
    prFun pr;
} List;
/* create a new, empty list */
List*	newList	(compFun,prFun);
/* destroy the list by deallocating used memory */
void 	delList	(List*);
/* return  the Nth element of the list if exists */
status 	nthInList	(List*,int,void**);

/* Insert element at a given position in the list */
status 	addListAt	(List*,int,void*);

/* remove the element located at a given position in list */
status 	remFromListAt	(List*,int,void**);

/* remove given element from given list */
status 	remFromList	(List*,void*);

/* display list elements as "[ e1 ... eN ]" */
status 	displayList	(List*);


/* sequencially call given function with each element of given list */
void	forEach		(List*,void(*)(void*));

/* compute and return the number of elements in given list */
int	lengthList	(List*);

/* add given element to given list according to compFun function */
status	addList	(List*,void*);

/* test whether the list contains given element */
Node*	isInList	(List*,void*);

#endif
