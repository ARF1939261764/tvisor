.PHONY : build

TESTNAME = guest0

# 可执行文件名
TARGET = main

ARCH = g
ABI = lp64d

# gcc类型
CROSS_COMPILE = riscv64-unknown-elf-

CC      = ${CROSS_COMPILE}gcc
LD      = ${CROSS_COMPILE}ld
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
READELF = ${CROSS_COMPILE}readelf

CFLAGS := -O0 -g3 -std=gnu99 -Wa,-march=rv64$(ARCH) -mabi=$(ABI)  -mcmodel=medany



LDFLAGS   ?= -Wl,--no-warn-rwx-segments -T ./startup/link.ld -nostartfiles -march=rv64$(ARCH) -mabi=$(ABI)  -mcmodel=medany


# 存放中间文件的路径
BUILD_DIR        = build
BUILD_DIR_CASE   = $(BUILD_DIR)/$(TESTNAME)
BUILD_DIR_CASE_O = $(BUILD_DIR)/$(TESTNAME)/objs

#存放.c 源文件的文件夹
SRC_DIR = \
	./startup/\
	./sys/sys\
	./sys/printf\
	./sdk/uart\
	./sdk/rocc\
	./case/${TESTNAME}

# 存放头文件的文件夹
INC_DIR = $(SRC_DIR)

# 在头文件路径前面加入-I
INCLUDE	= $(patsubst %, -I %, $(INC_DIR))

# 得到带路径的 .c 文件
CFILES := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))

# 得到不带路径的 .c 文件
CFILENDIR := $(notdir  $(CFILES))

# 将工程中的.c 文件替换成 ./build 目录下对应的目标文件 .o
COBJS = $(patsubst %, ./$(BUILD_DIR_CASE_O)/%, $(patsubst %.c, %.o, $(CFILENDIR)))

# 得到带路径的 .S 文件
SFILES := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.S))

# 得到不带路径的 .S 文件
SFILENDIR := $(notdir  $(SFILES))

# 将工程中的.S 文件替换成 ./build 目录下对应的目标文件 .o
SOBJS = $(patsubst %, ./$(BUILD_DIR_CASE_O)/%, $(patsubst %.S, %.o, $(SFILENDIR)))

# make 自动在源文件目录下搜索 .c 文件
VPATH = $(SRC_DIR)

all:$(BUILD_DIR) $(BUILD_DIR_CASE)/$(TARGET).elf
	$(OBJCOPY) -O binary -S $(BUILD_DIR_CASE)/$(TARGET).elf $(BUILD_DIR_CASE)/$(TARGET).bin
	$(OBJDUMP) -S -C -d $(BUILD_DIR_CASE)/$(TARGET).elf > $(BUILD_DIR_CASE)/$(TARGET).S
	$(READELF) -h -s -S $(BUILD_DIR_CASE)/$(TARGET).elf > $(BUILD_DIR_CASE)/$(TARGET).symtab


$(BUILD_DIR_CASE)/$(TARGET).elf :$(SOBJS) $(COBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR_CASE)
	@mkdir -p $(BUILD_DIR_CASE_O)


$(SOBJS) : $(BUILD_DIR_CASE_O)/%.o : %.S
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(COBJS) : $(BUILD_DIR_CASE_O)/%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	rm -rf ./$(BUILD_DIR_CASE)

cleanall:
	rm -rf ./$(BUILD_DIR)
