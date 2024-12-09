all:
	# compile
	g++ -std=c++17 -O3 -march=native -c ./main.cpp -o ./main.o
	g++ -o ./main.exe ./main.o
	
	# run executable
	./main.exe bcsstk13.mtx 10
	# ./main.exe toy.mtx 10

	# clean unnecessary binaries/executables
	rm ./main.exe ./main.o
