#shell variables no space
all:
	g++ -c -fopenmp ./mainS.cpp -o ./mainS.o -std=c++14
	g++ -o ./mainS ./mainS.cpp
	
	g++ -c -fopenmp ./mainS1.cpp -o ./mainS1.o -std=c++14
	g++ -o ./mainS1 ./mainS1.cpp
	
	g++ -c -fopenmp ./mainD.cpp -o ./mainD.o -std=c++14
	g++ -o ./mainD ./mainD.cpp

clean:
	rm ./main.exe ./main.o
	rm *.csv
