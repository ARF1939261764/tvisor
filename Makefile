.PHONY : default clean  cleanall run debug init

QEMU := ./qemu/qemu-10.2.0-rc1/build/qemu-system-riscv64 

default:
	@echo "build tvisor..."
	@mkdir -p ./build/
	@+make -C ./build/ -s
	@echo "build opensbi..."
	@+make -C ./opensbi/opensbi_v1p7/ PLATFORM=generic CROSS_COMPILE=riscv64-unknown-linux-gnu- FW_PAYLOAD_PATH=../../build/tvisor.bin all -s

clean:
	make -C ./build/ -s clean

cleanall:
	make -C ./build/ -s clean
	make -C ./opensbi/opensbi_v1p7/ -s clean
	make -C ./qemu/qemu-10.2.0-rc1/ -s clean

run:
	${QEMU} -M virt -cpu rv64,svpbmt=on -m 256M -nographic -bios ./opensbi/opensbi_v1p7/build/platform/generic/firmware/fw_payload.bin

debug:
	${QEMU} -M virt -cpu rv64,svpbmt=on -m 256M -nographic -bios ./opensbi/opensbi_v1p7/build/platform/generic/firmware/fw_payload.bin -s -S 

config:
	mkdir -p ./build/
	cmake -B ./build/ -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=1


init:
	@mkdir -p ./build/
	@cmake -B ./build/ -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	@+make -C ./build/ -s
	@+make -C ./opensbi/opensbi_v1p7/ PLATFORM=generic CROSS_COMPILE=riscv64-unknown-linux-gnu- FW_PAYLOAD_PATH=../../build/tvisor.bin all -s
	@cd ./qemu/qemu-10.2.0-rc1/ &&\
	./configure  --enable-debug --target-list=riscv64-softmmu -j 16 && \
	make -j 16 && \
	cd ../../


