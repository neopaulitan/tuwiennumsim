all: a1 a3 a4

a1:
	g++ -c ./task1/task1.C -o ./task1/task1.o
	g++ -o ./a1 ./task1/task1.o
	
a3:
	g++ -c ./task3/num3.cpp -o ./task3/num3.o -std=c++11
	g++ -o ./a3 ./task3/num3.o
	
a4:
	g++ -c ./task4/num4.cpp -o ./task4/num4.o -std=c++11
	g++ -o ./a4 ./task4/num4.o

clean:
	rm -f task1 task1.o COMPUTER.dat *.png
