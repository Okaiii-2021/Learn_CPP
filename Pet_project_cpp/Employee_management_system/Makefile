output: main.o system_app.o
	g++ main.o system_app.o -o output

main.o: main.cpp
	g++ -c main.cpp

system_app.o: system_app.cpp system_app.h Header.h Employeee.h
	g++ -c system_app.cpp

clean:
	del *.o output.exe