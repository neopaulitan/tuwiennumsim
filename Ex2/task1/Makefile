all:
	# compile
	g++ -std=c++17 -O3 -march=native -c ./main.cpp -o ./main.o
	g++ -o ./main.exe ./main.o
	
	# run executable
	./main.exe > triad_data.txt

	# clean unnecessary binaries/executables
	rm ./main.exe ./main.o
