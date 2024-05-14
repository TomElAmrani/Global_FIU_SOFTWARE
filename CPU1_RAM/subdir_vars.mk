################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../28004x_generic_ram_lnk.cmd 

LIB_SRCS += \
C:/ti/c2000/C2000Ware_5_02_00_00/driverlib/f28004x/driverlib/ccs/Debug/driverlib.lib 

SYSCONFIG_SRCS += \
../driverlib_cc_f28004x.sysconfig 

C_SRCS += \
../ACS712.c \
../DigiPot.c \
./syscfg/board.c \
./syscfg/c2000ware_libraries.c \
../interface.c \
../lab_main.c \
../volt_divider.c 

GEN_FILES += \
./syscfg/board.c \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt \
./syscfg/c2000ware_libraries.c 

GEN_MISC_DIRS += \
./syscfg 

C_DEPS += \
./ACS712.d \
./DigiPot.d \
./syscfg/board.d \
./syscfg/c2000ware_libraries.d \
./interface.d \
./lab_main.d \
./volt_divider.d 

GEN_OPTS += \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt 

OBJS += \
./ACS712.obj \
./DigiPot.obj \
./syscfg/board.obj \
./syscfg/c2000ware_libraries.obj \
./interface.obj \
./lab_main.obj \
./volt_divider.obj 

GEN_MISC_FILES += \
./syscfg/board.h \
./syscfg/board.cmd.genlibs \
./syscfg/board.json \
./syscfg/pinmux.csv \
./syscfg/adc.dot \
./syscfg/c2000ware_libraries.cmd.genlibs \
./syscfg/c2000ware_libraries.h \
./syscfg/clocktree.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg" 

OBJS__QUOTED += \
"ACS712.obj" \
"DigiPot.obj" \
"syscfg\board.obj" \
"syscfg\c2000ware_libraries.obj" \
"interface.obj" \
"lab_main.obj" \
"volt_divider.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\board.h" \
"syscfg\board.cmd.genlibs" \
"syscfg\board.json" \
"syscfg\pinmux.csv" \
"syscfg\adc.dot" \
"syscfg\c2000ware_libraries.cmd.genlibs" \
"syscfg\c2000ware_libraries.h" \
"syscfg\clocktree.h" 

C_DEPS__QUOTED += \
"ACS712.d" \
"DigiPot.d" \
"syscfg\board.d" \
"syscfg\c2000ware_libraries.d" \
"interface.d" \
"lab_main.d" \
"volt_divider.d" 

GEN_FILES__QUOTED += \
"syscfg\board.c" \
"syscfg\board.opt" \
"syscfg\c2000ware_libraries.opt" \
"syscfg\c2000ware_libraries.c" 

C_SRCS__QUOTED += \
"../ACS712.c" \
"../DigiPot.c" \
"./syscfg/board.c" \
"./syscfg/c2000ware_libraries.c" \
"../interface.c" \
"../lab_main.c" \
"../volt_divider.c" 

SYSCONFIG_SRCS__QUOTED += \
"../driverlib_cc_f28004x.sysconfig" 


