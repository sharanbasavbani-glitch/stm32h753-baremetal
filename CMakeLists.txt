cmake_minimum_required(VERSION 3.20)

project(STM32H753_Firmware LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(MCU_FLAGS
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

add_executable(firmware
    src/main.c
    startup/startup_stm32h753.s
)

target_compile_options(firmware PRIVATE
    ${MCU_FLAGS}
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
)

target_link_options(firmware PRIVATE
    ${MCU_FLAGS}
    -T${CMAKE_SOURCE_DIR}/linker/stm32h753.ld
    -Wl,--gc-sections
    -Wl,--build-id=none
    -Wl,-Map=firmware.map
    -nostartfiles
)

set_target_properties(firmware PROPERTIES
    SUFFIX ".elf"
)