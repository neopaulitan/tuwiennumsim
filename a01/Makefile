all: task1 task3 task4

task1:
	g++ -c task1.C -o ./task1.o
	g++ -o ./task1 ./task1.o
	
task3:
	g++ -c num3.cpp -o ./num3.o -std=c++11
	g++ -o ./task3 ./num3.o
	
task4:
	g++ -c num4.cpp -o ./num4.o -std=c++11
	g++ -o ./task4 ./num4.o

clean:
	rm -f task1 task1.o COMPUTER.dat *.png
