.PHONY: all

all: test_rtu_master
test_rtu_master : ZDSM.o test_rtu_master.o modbus.o modbus-rtu.o modbus-data.o 
	gcc ZDSM.o test_rtu_master.o modbus.o modbus-rtu.o modbus-data.o -o test_rtu_master

ZDSM485.o : ZDSM.cpp
	gcc ZDSM.cpp -cpp -I.
test_rtu_master.o : test_rtu_master.cpp
	gcc test_rtu_master.cpp -c -I.
modbus.o : modbus.c
	gcc modbus.c -c -I.
modbus-rtu.o : modbus-rtu.c
	gcc modbus-rtu.c -c -I.
modbus-data.o : modbus-data.c
	gcc modbus-data.c -c -I.

clean:
	rm -rf *.o *.exe

