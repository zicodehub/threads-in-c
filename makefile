main: main.c
	gcc main.c -lpthread -o launch
	
	./launch