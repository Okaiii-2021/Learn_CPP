output: main.o calculator.o
	g++ main.o calculator.o -o output

main.o: main.cpp
	g++ -c main.cpp

calculator.o: calculator.cpp calculator.h
	g++ -c calculator.cpp

clean:
	del *.o output.exe
