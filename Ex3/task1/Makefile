all:
	# compile
	g++ -std=c++17 -O3 -march=native -c ./main.cpp -o ./main.o
	g++ -o ./main.exe ./main.o
	
	# run executable
	# ./main.exe toy.mtx 10
	rm -rf ./res_eor.txt;
	rm -r ./Anorm_eor.txt;
	for i in 10 20 30 40 60 80 100 200 300 400 600 800 1000 2000 3000 4000 6000 8000 10000 20000 30000 40000 60000 80000 100000; do \
		./main.exe bcsstk13.mtx $$i; \
	done

	python ./plot.py

	# clean unnecessary binaries/executables
	rm ./main.exe ./main.o
