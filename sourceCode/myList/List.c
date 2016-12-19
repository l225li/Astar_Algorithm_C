//
//  List.c
//  List
//
//  Created by Lin Li on 11/4/16.
//  Copyright © 2016 Lin. All rights reserved.
//

#include <stdio.h>
#include "List.h"

List* newList(compFun comp,prFun pr){
    List* result = (List*) malloc(sizeof(List));
    if(!result){
        return 0;
    }
    result->comp = comp;
    result->pr = pr;
    result->nelts = 0;
    return result;
}

void delList(List* l){
    while(l->head){
        Node * tmp = l->head;
        l->head = tmp->next;
        free(tmp);
    }
    free(l);
}

status nthInList(List* l,int n,void** e){
    int i;
    if(n > l->nelts || n < 1){
        return ERRINDEX;
    }
    Node * current = l->head;
    for(i=1;i<n;i++){
        Node * tmp1 = current;
        current = tmp1->next;
        free(tmp1);
    }
    e = current->val;
    free(current);
    return OK;
}

status addListAt(List* l,int p,void* e){
    
    if(p > (l->nelts)+1 || p < 1) return ERRINDEX;
    Node * tmp = (Node *) malloc(sizeof(Node));
    if(!tmp) return ERRALLOC;
    if(p == 1){
        tmp->next = l->head;
        l->head = tmp;
        l->nelts++;
    }
    else{
        Node * current = l->head;
        int i  = 1;
        while(i < p-1){
            current = current->next;
            i++;
            if(i==p-1){
                tmp->next = current->next;
                current->next = tmp;
                l->nelts++;
            }
        }
    }
    tmp->val = e;
    l->nelts++;
    return OK;
}

status remFromListAt	(List* l,int p,void** e){
    if (p > l->nelts || p < 1) return ERRINDEX;
    Node * tmp;
    if (p==1){
        tmp = l->head;
        l->head = tmp->next;
        free(tmp);
        l->nelts--;
    }else{
        Node * current = l->head;
        Node * previous;
        int i = 1;
        while(i < p){
            previous = current;
            current = current->next;
            i++;
            if(i==p){
                tmp = current;
                previous->next = tmp->next;
                free(tmp);
                l->nelts--;
            }
        }
        
    }
    return OK;
    
}
status 	remFromList	(List* l,void* e){
    if (!l->comp) return ERRUNABLE;
    Node * current = l->head;
    Node * previous = NULL;
    Node * tmp;
    while(current->next){
        if(l->comp(e, current->val) == 0){
            tmp = current;
            previous->next = tmp->next;
            free(tmp);
            l->nelts--;
            return OK;
        }
        previous = current;
        current = current->next;
    }
    
    return ERRABSENT;
}
status 	displayList	(List* l){
    if (!l->comp) return ERRUNABLE;
    Node * current = l->head;
    printf("[ ");
    while(current){
        l->pr(current->val);
        current = current->next;
        printf(" ");
    }
    printf("]");
    return OK;
    
}
void forEach (List* l,void(*f)(void *)){
    Node * current = l->head;
    while(current){
        f(current->val);
        current = current->next;
    }
}
int	lengthList	(List* l){
    Node * current = l->head;
    int count = 1;
    while(current->next){
        current=current->next;
        count++;
    }
    return count;
}

status	addList	(List* l,void* e){
    Node * tmp = (Node *) malloc(sizeof(Node));
    if(!tmp) return ERRALLOC;
    if(!l->comp) return ERRUNABLE;
    if(!l->head){
        tmp->val =e;
        l->head = tmp;
        l->nelts++;
        return OK;
    }
    
    Node * current = l->head;
    Node * previous = NULL;
    while(current){
        if(l->comp(current->val,e) > 0){
            tmp->val=e;
            tmp->next = current;
            if(previous){
                previous->next = tmp;}
            l->nelts++;
            return OK;
        }
        if(!current->next){break;}
        previous = current;
        current = current->next;
    }
    tmp->val = e;
    current->next = tmp;
    l->nelts++;
    return OK;
}

Node*	isInList	(List* l,void* e){
    Node * result = (Node *)malloc(sizeof(Node));
    if(!l->head) return result->val = (void *)0;
    if (l->comp(e,l->head->val)==0) return result->val = (void *)1;
    Node * current = l->head;
    Node * previous = NULL;
    while(current->next){
        
        if(l->comp(e,current->val) == 0){
            return previous;
        }
        previous = current;
        current = current->next;
    }
    return result->val = (void *)0;
}
