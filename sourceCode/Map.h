//
//  Map.h
//  List
//
//  Created by Lin Li on 17/11/16.
//  Copyright © 2016 Lin. All rights reserved.
//

#ifndef Map_h
#define Map_h
#include <stdio.h>
#include "List.h"

/** City structure **/
typedef struct City{
    char name[20];
    int distFromStart;
    int distToGoal;
    int lat;
    int lgt;
    struct City * ptr;
    List * neighbours;
}City;

/** Neighbour structure: neighbour city with its distance from current city **/
typedef struct neighbour{
    City * city;
    int distance;
}neighbour;

/** Part of the estimation function of cost, to calculate the estimated distance from current city to goal city **/
int h_of_n(City *, City *);

/** Cost estimation function**/
int f_of_n(City *);

/** Comparison function to compare the distance between neighbours **/
int compDistance(void *, void *);

/** Comparison function to compare the distance traveled **/
int comp_g_of_n(void *, void *);

/** Comparison function to compare the total distance from start to end if taking the current path **/
int comp_f_of_n(void *, void *);

/** Comparison function to compare strings **/
int compString(void *, void *);

/** Display function to display the cities **/
void prCities(void * );

/** Display function to display the neighbouurs**/
void prNeighbours(void *);

/** Initialization function of a City **/
City * init_City(char*,int, int);

/** Initialization function of a neighbour **/
neighbour * init_neighbour(City *,int);

/** Find the address of a city by the given name of the city **/
City * find_city(List * l, char[20]);

/** From the given map filename, store the corresponding file into a map implemented by a list of cities with lists of neighbours **/
List * map_to_list(char [200]);

#endif /* Map_h */
