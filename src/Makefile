.PHONY: all

all: test_rtu_master
test_rtu_master : test_rtu_master.o modbus.o modbus-rtu.o modbus-data.o 
	gcc test_rtu_master.o modbus.o modbus-rtu.o modbus-data.o -o test_rtu_master

test_rtu_master.o : test_rtu_master.c
	gcc test_rtu_master.c -c -I.
modbus.o : modbus.c
	gcc modbus.c -c -I.
modbus-rtu.o : modbus-rtu.c
	gcc modbus-rtu.c -c -I.
modbus-data.o : modbus-data.c
	gcc modbus-data.c -c -I.

clean:
	rm -rf *.o *.exe

