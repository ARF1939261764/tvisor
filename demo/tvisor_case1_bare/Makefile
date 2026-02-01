TESTNAME := guest0

all:
	make -C ./firmware/ TESTNAME=${TESTNAME}
	xxd -i ./firmware/build/${TESTNAME}/main.bin > guest0.c

clean:
	make -C ./firmware/ TESTNAME=${TESTNAME} clean
