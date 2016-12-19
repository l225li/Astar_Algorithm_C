//
//  main.c
//  List
//
//  Created by Lin Li on 18/11/16.
//  Copyright © 2016 Lin. All rights reserved.
//

#include <stdio.h>
#include "Map.h"

int main(){

    List * all_cities = map_to_list("/Users/Lin/Desktop/Specialization Period/Advanced C/Project/Astar/Astar/FRANCE.MAP");
    List * OPEN = newList(comp_f_of_n, prCities);
    List * CLOSED = newList(compString, prCities);
    City * start = find_city(all_cities, "Rennes");
    City * goal = find_city(all_cities, "Lyon");
    
    addList(OPEN, start);
    start->distFromStart = 0;
    start->distToGoal=h_of_n(start, goal);
    
    while(OPEN->head){
        void * e;
        remFromListAt(OPEN, 1, &e);
        addList(CLOSED, e);
        City * n = (City *) e;
        
        if(n == goal){
            puts("\n");
            puts("Success!\n");
            puts("The path found:\n");
            while(n){
                printf("%s<-",n->name);
                n = n ->ptr;
            }
            puts("\n");
            return 0;
        }
        
        Node * tmp = n->neighbours->head;
        
        while(tmp){
            
            City * succ = ((neighbour *)tmp->val)->city;
            int distance =((neighbour *)tmp->val)->distance;
            int distance_so_far = n->distFromStart + distance;
            
            if((isInList(OPEN, succ)!=0) ||
              ((isInList(CLOSED, succ)!=0) &&
                    (distance_so_far > succ->distFromStart)))
            {
                    tmp = tmp->next;
            }
            else{
                if(isInList(OPEN, succ)!=0){
                    remFromList(OPEN, succ);
                }
                if(isInList(CLOSED, succ)!=0){
                    remFromList(CLOSED, succ);
                }
                succ->distFromStart = distance_so_far;
                succ->ptr = n;
                addList(OPEN, succ);
                tmp = tmp->next;
            }
        }
    }
    
    puts("failure");
    return 0;
}
