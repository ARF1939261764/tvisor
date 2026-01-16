all:
	make -C ./firmware/
	xxd -i ./firmware/build/smoke/main.bin > guest.c

clean:
	make -C ./firmware/ clean
