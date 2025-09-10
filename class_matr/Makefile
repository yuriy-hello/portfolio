matr: main.o matrix.o
	g++ main.o matrix.o -o matr
main.o: main.cpp matrix.h
	g++ main.cpp -c -o main.o
matrix.o: matrix.cpp matrix.h
	g++ matrix.cpp -c -o matrix.o
clean:
	rm *.o
