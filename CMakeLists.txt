cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
# 项目名称和语言支持
project(tvisor
  LANGUAGES C ASM
  VERSION 0.1.0
)

# set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Verbose Makefile" FORCE)


set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_AR riscv64-unknown-linux-gnu-ar)
set(CMAKE_C_COMPILER riscv64-unknown-linux-gnu-gcc)
set(CMAKE_OBJCOPY riscv64-unknown-linux-gnu-objcopy)
set(CMAKE_OBJDUMP riscv64-unknown-linux-gnu-objdump)
set(CMAKE_ASM_COMPILER riscv64-unknown-linux-gnu-gcc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
 
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
 
endif()

function(add_bin_target target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin
        COMMENT "✅ Generating binary file: ${target}.bin"
        VERBATIM  # 关键：防止路径含空格/特殊字符出错，必加
    )
endfunction()

function(add_asm_target target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -S -C -D $<TARGET_FILE:${target}> > ${CMAKE_CURRENT_BINARY_DIR}/${target}.S
        COMMENT "✅ Generating assembly file: ${target}.S"
        VERBATIM  # 关键：防止路径含空格/特殊字符出错，必加
    )
endfunction()

add_library(ProjectCommonConfig INTERFACE)

target_include_directories(ProjectCommonConfig INTERFACE
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

target_link_options(ProjectCommonConfig INTERFACE
  -static -pie -Wl,--no-warn-rwx-segments -T ${PROJECT_SOURCE_DIR}/tvisor/arch/riscv/startup/link.ld -nostartfiles 
  -lc -lgcc  -march=rv64gch -mabi=lp64d
)

target_compile_options(ProjectCommonConfig INTERFACE
   -O0 -g3 -funroll-loops -fpeel-loops -fgcse-sm -fgcse-las -static -std=gnu99 
                                        -fno-common -fno-builtin-printf -march=rv64gch -mabi=lp64d  -mcmodel=medany
)
 
# 收集源文件（推荐手动列出，避免遗漏或误包含）
set(DIR_TARGET_SRC
    ./tvisor/arch/riscv/freertos/port.c
    ./tvisor/arch/riscv/freertos/port_asm.S
    ./tvisor/arch/riscv/startup/crt.S
    ./tvisor/freertos/queue.c
    ./tvisor/freertos/list.c
    ./tvisor/freertos/tasks.c
    ./tvisor/freertos/event_groups.c
    ./tvisor/freertos/timers.c
    ./tvisor/freertos/stream_buffer.c
    ./tvisor/freertos/croutine.c
    ./tvisor/freertos/heap_5.c
    ./tvisor/tvisor_printf.c
    ./tvisor/tvisor_mmu.c
    ./tvisor/tvisor_guest_page_fault.c
    ./tvisor/tvisor_dev_uart16550.c
    ./tvisor/tvisor_dev_clint.c
    ./tvisor/tvisor_dev_mem.c
    ./tvisor/tvisor_dev_stimer.c
    ./tvisor/tvisor_dev_plic.c
    ./tvisor/tvisor_dev.c
    ./tvisor/tvisor.c
)

add_library(tvisor STATIC 
  ${DIR_TARGET_SRC}
)

# 构建可执行文件
add_executable(tvisor_case0_smoke
  ./demo/tvisor_case0_smoke/system.c
  ./demo/tvisor_case0_smoke/main.c
)

# 构建可执行文件
add_executable(tvisor_case1_bare
  ./demo/tvisor_case1_bare/system.c
  ./demo/tvisor_case1_bare/guest0.c
  ./demo/tvisor_case1_bare/main.c
)

# 构建可执行文件
add_executable(tvisor_case2_freertos
  ./demo/tvisor_case2_freertos/system.c
  ./demo/tvisor_case2_freertos/main.c
)


get_property(ALL_TARGETS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

# 4. 循环遍历，给所有target生成bin文件
foreach(target IN LISTS ALL_TARGETS)
  
  get_target_property(target_type ${target} TYPE)

  if(target_type STREQUAL "EXECUTABLE")
    add_bin_target(${target})
    add_asm_target(${target})
  endif()

  if(target_type STREQUAL "STATIC_LIBRARY")
    target_link_libraries(${target} PRIVATE ProjectCommonConfig)
  endif()
  if(target_type STREQUAL "EXECUTABLE")
    target_link_libraries(${target} PRIVATE ProjectCommonConfig tvisor)
  endif()
  
endforeach()
