#shell variables no space
all:
	g++ -c ./main.cpp -o ./main.o -std=c++14
	g++ -o ./main.exe ./main.cpp

iteration:		
	#no spaces after backslash in for loop
	for i in 10 100 500 1000 5000; do \
		normcsv=./normiter_$${i}.csv;\
		terminalout=./output.log;\
		./main.exe iternum_$${i} 128 $$i 0.0 0.0 0.5 0.5 0.1 > $${terminalout};\
		grep -m 1 'iters' $${terminalout} | awk -F 'iters:' '{print $$2}' | tr -d ' '  > $${normcsv};\
		grep -m 2 '=' $${terminalout} | awk -F '=' '{print $$2}' | tr -d ' ' >> $${normcsv};\
		rm $${terminalout};\
	done

	python ./plotty.py

clean:
	rm ./main.exe ./main.o
	rm *.csv
