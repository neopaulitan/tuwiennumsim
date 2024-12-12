all:
	# compile
	g++ -std=c++17 -O3 -march=native -c ./main.cpp -o ./main.o
	g++ -o ./main.exe ./main.o
	
	# run executable
	# ./main.exe toy.mtx 10
	rm -rf ./res_eor.txt;
	rm -r ./Anorm_eor.txt;
	for i in 10 100 500 1000 5000 10000 50000 100000; do \
		./main.exe bcsstk13.mtx $$i; \
	done

	# clean unnecessary binaries/executables
	rm ./main.exe ./main.o
