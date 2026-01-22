all:
	make -C ./firmware/ TESTNAME=guest0
	xxd -i ./firmware/build/guest0/main.bin > guest0.c

clean:
	make -C ./firmware/ clean
