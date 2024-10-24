all_O0-3:
	for i in 0 1 2 3; do \
		echo "thing1"; \
		g++ -c ./num4.cpp -O$$i -o ./num4_$$i.o -std=c++11; \
		echo "thing2"; \
		g++ -o ./task4_$$i ./num4_$$i.o; \
	done

	python3 ./num4.py
