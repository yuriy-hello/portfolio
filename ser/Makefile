ser: ser.o ser_func.o
		g++ ser.o ser_func.o -o ser

ser.o: ser.cpp ser_func.h
		g++ ser.cpp -c -o ser.o
		
ser_func.o: ser_func.cpp ser_func.h ser_func_const.h
		g++ ser_func.cpp -c -o ser_func.o
		
clean:
		rm *.o ./ser
		
cl: client_for_ser.o ser_func.o
		g++ client_for_ser.o ser_func.o -o cl
		
client_for_ser.o: client_for_ser.cpp ser_func.h
		g++ client_for_ser.cpp -c -o client_for_ser.o
