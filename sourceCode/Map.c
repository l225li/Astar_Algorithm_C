//
//  Map.c
//  List
//
//  Created by Lin Li on 18/11/16.
//  Copyright © 2016 Lin. All rights reserved.
//

#include <stdio.h>
#include "Map.h"

/** constant infinity number set to 9999 **/
int infinity = 9999;


/*************************************************************
 * Part of the estimation function of cost, to calculate the estimated distance from current city to goal city
 * @param c current citty
 * @param goal goal city
 * @return the estimated distance calculated using latitude and longitude
 *************************************************************/
int h_of_n(City * c,City * goal){
    return (abs(c->lat - goal->lat) + abs(c->lgt - goal->lgt))/4;
}


/*************************************************************
 *Cost estimation function
 * @param c the current city
 * @return the estimated cost of distance to travel if using the current path
 *************************************************************/
int f_of_n(City * c){
    return c->distFromStart + c->distToGoal;
}


/*************************************************************
 * Comparison function to compare the distance between neighbours
 * @param n1 is the first neighbour to compare
 * @param n2 is the second neighbour to compare
 * @return <0 if n1 is less than n2
 * @return 0 if n1 equals n2
 * @return >0 otherwise
 *************************************************************/
int compDistance(void * n1, void * n2){
    return ((neighbour*)n1)->distance - ((neighbour*)n2)->distance;
}


/*************************************************************
 * Comparison function to compare the distance traveled between 2 cities
 * @param c1 is the first city to compare
 * @param c2 is the second city to compare
 * @return <0 if c1 is less than c2
 * @return 0 if c1 equals c2
 * @return >0 otherwise
 *************************************************************/
int comp_g_of_n(void * c1, void * c2){
    return (((City *)c1)->distFromStart - ((City *)c2)->distFromStart);
}


/*************************************************************
 * Comparison function to compare the total distance from start to end if taking the current path
 * @param c1 is the first city to compare
 * @param c2 is the second city to compare
 * @return <0 if c1 is less than c2
 * @return 0 if c1 equals c2
 * @return >0 otherwise
 *************************************************************/
int comp_f_of_n(void * c1, void * c2){
    City * n1 = (City *)c1;
    City * n2= (City *)c2;
    return f_of_n(n1)-f_of_n(n2);
}


/*************************************************************
 * Function to compare two elements (strings) : based on strcmp
 * @param s1 the first string to compare
 * @param s2 the second string to compare
 * @return <0 if s1 is less than s2
 * @return 0 if s1 equals s2
 * @return >0 otherwise
 *************************************************************/
int compString (void * s1, void * s2) {
    return (int)strcmp(((City *)s1)->name, ((City *)s2)->name);
}


/*************************************************************
 * Display function to display the cities
 * @param e the city to display
 *************************************************************/
void prCities(void * e){
    City * c = (City *)e;
    printf("%s %d %d %d\n", c->name, c->lat,c->lgt,c->neighbours->nelts);
    forEach(c->neighbours, prNeighbours);
}


/*************************************************************
 * Display function to display the neighbouurs
 * @param e the neighbour to display
 *************************************************************/
void prNeighbours(void * e){
    neighbour * n = (neighbour *)e;
    printf("%d\n",n->distance);
}


/*************************************************************
 * Initialization function of a City
 * @param name name of the city
 * @param lat latitude of the city
 * @param lgt longitude of the city
 * @return the initialized city, with distanceFromStart of infinity
 *************************************************************/
City * init_City(char* name, int lat, int lgt){
    City * c = (City *) malloc(sizeof(City));
    strcpy(c->name,name);
    c->lat = lat;
    c->lgt = lgt;
    c->distFromStart = infinity;
    c->neighbours = newList(compDistance, prCities);
    c->neighbours->comp=compDistance;
    return c;
}


/*************************************************************
 * Initialization function of a neighbour
 * @param city city the neighbour is of 
 * @param distance distance between city and neighbour
 * @return initialized neighbour
 *************************************************************/
neighbour * init_neighbour(City * city, int distance){
    
    neighbour * n = (neighbour *) malloc(sizeof(neighbour));
    n->city = city;
    n->distance = distance;
    return n;
}


/*************************************************************
 * Find the address of a city by its name in given list of cities
 * @param l list of cities
 * @param name name of the city to be found
 * @return 0 if city is not found
 * @return NULL if list given is empty/NULL
 * @return city * if found
 *************************************************************/
City * find_city(List * l,char name[20]){
    if(!l->head) return NULL;
    Node * current = l->head;
    while(current){
        
        if(strcmp(((City *)(current->val))->name, name) == 0){
            return current->val;
        }
        current=current->next;
    }
    return 0;
}


/*************************************************************
 * From the given map filename, store the corresponding file into a map implemented by a list of cities with lists of neighbours
 * @param filepath filepath of the file to be read
 * @return a list of cities read from the file 
 * @return NULL if error
 *************************************************************/
List * map_to_list(char filepath[200]){
    
    /*Create a filereader and read the map data into a list of list*/
    FILE * fPointer;
    fPointer = fopen(filepath, "r");
    if (!fPointer) return NULL;
    char str_1[20];
    int num_1;
    int num_2;
    City * tmp_city = (City *)malloc(sizeof(City));
    List * all_cities = newList(compString, prCities);
    
    rewind(fPointer);
    int nItems = 0;
    
    while(nItems!=-1){
        
        nItems = fscanf(fPointer,"%s %d %d\n", str_1, &num_1, &num_2);
        City * existing = find_city(all_cities, str_1);
        
        if (nItems == 3){
            if(existing == 0){
                tmp_city = init_City(str_1, num_1, num_2);
                addList(all_cities, tmp_city);
            }else{
                tmp_city = existing;
                tmp_city->lat = num_1;
                tmp_city->lgt = num_2;
            }
        }
        
        else if (nItems == 2){
            neighbour * tmp_nb = (neighbour *)malloc(sizeof(neighbour));
            if (existing==0){
                City * nb_city = init_City(str_1, -1, -1); //if city is not inside list yet, initial a temp one
                addList(all_cities, nb_city);
                tmp_nb = init_neighbour(nb_city,num_1);
                addList(tmp_city->neighbours, tmp_nb);//add the read neighbour to list of neighbours
                
            }else{
                tmp_nb=init_neighbour(existing, num_1);
                addList(tmp_city->neighbours, tmp_nb);
            }
        }
    }
    
    fclose(fPointer);
    puts("For Each: cityname, lat, lgt, number of neighbours\n");
    forEach(all_cities, prCities);
    
    return all_cities;
}


