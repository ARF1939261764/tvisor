cmake_minimum_required(VERSION 3.0)

# 项目名称和语言支持
project(tvisor
  LANGUAGES C ASM
  VERSION 0.1.0
)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER riscv64-unknown-linux-gnu-gcc)
set(CMAKE_OBJCOPY riscv64-unknown-linux-gnu-objcopy)
set(CMAKE_OBJDUMP riscv64-unknown-linux-gnu-objdump)
set(CMAKE_ASM_COMPILER riscv64-unknown-linux-gnu-gcc)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# 收集源文件（推荐手动列出，避免遗漏或误包含）
set(DIR_TARGET_SRC
    tvisor/arch/riscv/freertos/port.c
    tvisor/arch/riscv/freertos/port_asm.S
    tvisor/arch/riscv/startup/crt.S
    tvisor/freertos/queue.c
    tvisor/freertos/list.c
    tvisor/freertos/tasks.c
    tvisor/freertos/event_groups.c
    tvisor/freertos/timers.c
    tvisor/freertos/stream_buffer.c
    tvisor/freertos/croutine.c
    tvisor/freertos/heap_5.c
    tvisor/tvisor_printf.c
    tvisor/tvisor_mmu.c
    tvisor/tvisor.c
    main/system.c
    main/main.c
)

# 构建可执行文件
add_executable(tvisor
  ${DIR_TARGET_SRC}
)

target_include_directories(tvisor PUBLIC 
    ${PROJECT_SOURCE_DIR}/main/include
    ${PROJECT_SOURCE_DIR}/tvisor/include
    ${PROJECT_SOURCE_DIR}/tvisor/freertos/include
    ${PROJECT_SOURCE_DIR}/tvisor/arch/riscv/freertos/include
    ${PROJECT_SOURCE_DIR}/tvisor/arch/riscv/startup/include
    /home/tangf/04_softs/riscv-tools/bin/../lib/gcc/riscv64-unknown-linux-gnu/12.2.0/include
    /home/tangf/04_softs/riscv-tools/bin/../lib/gcc/riscv64-unknown-linux-gnu/12.2.0/include-fixed
    /home/tangf/04_softs/riscv-tools/bin/../lib/gcc/riscv64-unknown-linux-gnu/12.2.0/../../../../riscv64-unknown-linux-gnu/include
    /home/tangf/04_softs/riscv-tools/bin/../sysroot/usr/include
)

target_compile_options(tvisor PRIVATE   -O0 -g3 -funroll-loops -fpeel-loops -fgcse-sm -fgcse-las -static -std=gnu99 
                                        -fno-common -fno-builtin-printf -Wa,-march=rv64gch -mabi=lp64d  -mcmodel=medany)

target_link_options(tvisor PRIVATE -static -pie -Wl,--no-warn-rwx-segments -T ${PROJECT_SOURCE_DIR}/tvisor/arch/riscv/startup/link.ld -nostartfiles  -lc -lgcc -march=rv64ghc -mabi=lp64d)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating binary file: ${CMAKE_PROJECT_NAME}.bin"
)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${CMAKE_PROJECT_NAME}> > ${CMAKE_PROJECT_NAME}.S
    COMMENT "Generating dump file: ${CMAKE_PROJECT_NAME}.S"
)
