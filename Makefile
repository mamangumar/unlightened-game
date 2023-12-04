all: compile link run

compile:
	g++ -c main.cpp -I "C:\Users\santi\Documents\libraries\SFML-2.5.1\include" 

link: 
	g++ main.o -o main -L "C:\Users\santi\Documents\libraries\SFML-2.5.1\lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

clean: 
	rm -f main *.o

run: 
	.\main.exe