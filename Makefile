.PHONY : all run  debug config clean

QEMU := ./qemu/qemu-10.2.0-rc1/build/qemu-system-riscv64 

all:
	@echo "build tvisor..."
	@+make -C ./build/ -s
	@echo "build opensbi..."
	@+make -C ./opensbi/opensbi_v1p7/ PLATFORM=generic CROSS_COMPILE=riscv64-unknown-linux-gnu- FW_PAYLOAD_PATH=../../build/tvisor.bin all -s

clean:
	make -C ./build/ -s clean

cleanall:
	make -C ./build/ -s clean
	make -C ./opensbi/opensbi_v1p7/ -s clean

run:
	${QEMU} -M virt -cpu rv64,svpbmt=on -m 256M -nographic -bios ./opensbi/opensbi_v1p7/build/platform/generic/firmware/fw_payload.bin

debug:
	${QEMU} -M virt -cpu rv64,svpbmt=on -m 256M -nographic -bios ./opensbi/opensbi_v1p7/build/platform/generic/firmware/fw_payload.bin -s -S 

config:
	mkdir -p ./build/
	cmake -B ./build/ -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=1


