Arguments := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

main : main.c iss.c iss.h
	gcc -c iss.c
	gcc main.c iss.o -o main.o

clean : 
	rm -f *.o
