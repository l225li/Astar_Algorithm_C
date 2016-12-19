#makefile

Astar:  main.o Map.o List.o status.o
	gcc -o Astar main.o Map.o List.o status.o

main.o:  main.c Map.h 
	gcc -c -Wall -Wno-error main.c

Map.o:  Map.c List.h 
	gcc -c -Wall -Wno-error Map.c

List.o:  List.c status.h 
	gcc -c -Wall -Wno-error List.c

status.o:  status.c status.h 
	gcc -c -Wall -Wno-error status.c