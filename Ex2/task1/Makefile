all:
	# compile
	g++ -std=c++17 -O3 -march=native -c ./main.cpp -o ./main.o
	g++ -o ./main ./main.o
	
	# run executable
	./main > triad_data.txt

	# clean unnecessary binaries/executables
	rm main main.o
